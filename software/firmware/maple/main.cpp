#include "wirish.h"
#include "Wire.h"
#include <stdio.h>
#include <string.h>

#include "mixfft.h"
#include "MMA8452Q.h"

#define ADDR_DEVICE 0x1C
#define I2C_WRITE 0
#define I2C_READ  1

#define WINDOW_SIZE 128
#define AXES 3

double zeros[WINDOW_SIZE];

double accel_window[AXES][WINDOW_SIZE];

double fft_real[AXES][WINDOW_SIZE];
double fft_img[AXES][WINDOW_SIZE];

void who_am_i();

uint8 accel_write(uint8 addr, uint8 data);
uint8 accel_read(uint8 addr);
uint8 accel_multi_read(uint8 addr, uint8 num, uint8 data[]);

void printXYZ(int x, int y, int z);
void sendXYZ(int x, int y, int z);

void loop();

void setup() {
	for(int i = 0;i < WINDOW_SIZE;i++) {
		zeros[i] = 0;
	
		for(int j = 0;j < AXES;j++) {
			accel_window[j][i] = 0;
			fft_real[j][i] = 0;
			fft_img[j][i] = 0;
		}
	}
	   
	Wire.begin(9, 5);
	
	accel_write(CTRL_REG2, 1 << RST);
	
	delay(500);

	while((accel_read(CTRL_REG2) & (1 << RST)) != 0) {
		SerialUSB.println("Waiting for accel reset");
		delay(500);
	}
	
	accel_write(XYZ_DATA_CFG, 1 << FS1 | 1 << FS0);
	accel_write(CTRL_REG1, 1 << ACTIVE);	
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
	delay(500);
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
	delay(100);
}

void sendXYZ(int x, int y, int z) {
	SerialUSB.print(0xFF, BYTE);
	SerialUSB.print(0xFF, BYTE);

	SerialUSB.print((x & 0xFF00) >> 8, BYTE);
	SerialUSB.print((x & 0xFF), BYTE);

	SerialUSB.print((y & 0xFF00) >> 8, BYTE);
	SerialUSB.print((y & 0xFF), BYTE);

	SerialUSB.print((z & 0xFF00) >> 8, BYTE);
	SerialUSB.print((z & 0xFF), BYTE);	
}

void sendFFT() {
	SerialUSB.print(0xFF, BYTE);
	SerialUSB.print(0xFF, BYTE);
	
	// for(int i = 0;i < WINDOW_SIZE;i++) {
	// 	SerialUSB.print((fft_real[i] & 0xFF00) >> 8, BYTE);
	// 	SerialUSB.print((fft_real[i] & 0xFF), BYTE);
	// }
	// 
	// for(int i = 0;i < WINDOW_SIZE;i++) {
	// 	SerialUSB.print((fft_img[i] & 0xFF00) >> 8, BYTE);
	// 	SerialUSB.print((fft_img[i] & 0xFF), BYTE);
	// }
}

void loop() {
	uint8 data[AXES*2] = {0, 0, 0, 0, 0, 0};
	accel_multi_read(OUT_X_MSB, AXES*2, data);
		
	int16 accel_values[AXES] = {0, 0, 0};
	
	for(int i = 0;i < AXES;i++) {
		accel_values[i] = (data[i*2] << 4) | (data[i*2+1] >> 4);
		if(accel_values[i] & 0x0800) accel_values[i] |= 0xF000;
	}
	
	for(int i = 0;i < AXES;i++) {
		memmove(accel_window[i]+0, accel_window[i]+1, WINDOW_SIZE-1);
		accel_window[i][0] = accel_values[i];
		fft(WINDOW_SIZE, accel_window[i], zeros, fft_real[i], fft_img[i]);
	}
	
	sendFFT();
}

__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while(1) loop();

	return 1;
}
