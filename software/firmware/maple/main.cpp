#include "wirish.h"
#include "Wire.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mixfft.h"
#include "MMA8452Q.h"

#define ADDR_DEVICE 0x1C
#define I2C_WRITE 0
#define I2C_READ  1

#define WINDOW_SIZE 64
#define AXES 2

#define DOWNSAMPLE 1000

int tick;

int zeros[WINDOW_SIZE];

int accel_window[AXES][WINDOW_SIZE];

int fft_real[AXES][WINDOW_SIZE];
int fft_img[AXES][WINDOW_SIZE];
uint fft_amp[AXES][WINDOW_SIZE];

void who_am_i();

uint8 accel_write(uint8 addr, uint8 data);
uint8 accel_read(uint8 addr);
uint8 accel_multi_read(uint8 addr, uint8 num, uint8 data[]);

void printXYZ(int x, int y, int z);
void sendXYZ(int x, int y, int z);

void computeFFTAmp();

void loop();

void setup() {
	for(int i = 0;i < WINDOW_SIZE;i++) {
		zeros[i] = 0;

		for(int j = 0;j < AXES;j++) {
			accel_window[j][i] = 0;
			fft_real[j][i] = 0;
			fft_img[j][i] = 0;
			fft_amp[j][i] = 0;
		}
	}

	Wire.begin(9, 5);

	accel_write(CTRL_REG2, 1 << RST);

	delay(500);

	while((accel_read(CTRL_REG2) & (1 << RST)) != 0) {
		SerialUSB.println("Waiting for accel reset");
		delay(1);
	}

	accel_write(XYZ_DATA_CFG, 1 << FS1 | 1 << FS0);
	accel_write(CTRL_REG1, 0 << DR2 | 0 << DR1 | 0 << DR0| 1 << ACTIVE);

	delay(2000);
}

uint8 accel_write(uint8 addr, uint8 data) {
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(addr);
	Wire.send(data);
	return Wire.endTransmission();
}

void who_am_i() {
	uint8 res = accel_read(WHO_AM_I);
	SerialUSB.println(res, HEX);
}

uint8 accel_read(uint8 addr) {
	Wire.requestFrom(ADDR_DEVICE, 1);
	return Wire.receive();
}

uint8 accel_multi_read(uint8 addr, uint8 num, uint8 data[]) {
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(addr);
	Wire.endTransmission();

	uint8 rx_len = Wire.requestFrom(ADDR_DEVICE, num);
	for(int i = 0;i < num;i++) data[i] = Wire.receive();
	return rx_len;
}

void printXYZ(int x, int y, int z) {
	SerialUSB.print(x, DEC);
	SerialUSB.print("\t");
	SerialUSB.print(y, DEC);
	SerialUSB.print("\t");
	SerialUSB.println(z, DEC);
}

void sendXYZ(int x, int y, int z) {
	SerialUSB.print(0xFF, BYTE);
	SerialUSB.print(0xFF, BYTE);

	if(AXES > 0) {
		SerialUSB.print((x & 0xFF00) >> 8, BYTE);
		SerialUSB.print((x & 0xFF), BYTE);
	}
	
	if(AXES > 1) {
		SerialUSB.print((y & 0xFF00) >> 8, BYTE);
		SerialUSB.print((y & 0xFF), BYTE);
	}
	
	if(AXES > 2) {
		SerialUSB.print((z & 0xFF00) >> 8, BYTE);
		SerialUSB.print((z & 0xFF), BYTE);
	}
}

void sendFFT() {
	SerialUSB.print(0xFF, BYTE);
	SerialUSB.print(0xFF, BYTE);

	for(int i = 0;i < AXES;i++) {
		for(int j = 0;j < WINDOW_SIZE;j++) {
			SerialUSB.print((fft_amp[i][j] & 0xFF00) >> 8, BYTE);
			SerialUSB.print((fft_amp[i][j] & 0xFF), BYTE);
		}
	}
}

void loop() {
	if(tick%DOWNSAMPLE == 0) {
		uint8 data[AXES*2];
		for(int i = 0;i < AXES*2;i++) data[i] = 0;

		accel_multi_read(OUT_X_MSB, AXES*2, data);

		int16 accel_values[AXES];
		for(int i = 0;i < AXES;i++) accel_values[i] = 0;

		for(int i = 0;i < AXES;i++) {
			accel_values[i] = (data[i*2] << 4) | (data[i*2+1] >> 4);
			if(accel_values[i] & 0x0800) accel_values[i] |= 0xF000;
		}


		for(int i = 0;i < AXES;i++) {
			memmove(accel_window[i]+1, accel_window[i], (WINDOW_SIZE-1)*4);
			accel_window[i][0] = (int16)(0.2*accel_values[i]+0.8*accel_window[i][1]);
			fft(WINDOW_SIZE, accel_window[i], zeros, fft_real[i], fft_img[i]);
		}
		
		// sendXYZ(accel_window[0][0], accel_window[1][0], accel_window[2][0]);

		computeFFTAmp();
		sendFFT();
	} else {
		delayMicroseconds(1);
	}
	tick++;
}

void computeFFTAmp() {
	for(int i = 0;i < AXES;i++) {
		for(int j = 0;j < WINDOW_SIZE;j++) {
			fft_amp[i][j] = (abs(fft_real[i][j])+abs(fft_img[i][j]));
		}
	}
}

__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while(1) loop();

	return 1;
}
