#include "wirish.h"
#include "Wire.h"

#include "MMA8452Q.h"

// test i2c
#define ADDR_DEVICE 0x1C
#define WINDOW_SIZE 128

int16 accel_window[3][WINDOW_SIZE];
int16 fft_data[3][WINDOW_SIZE];

void setup() {
	
	for(int i = 0;i < WINDOW_SIZE;i++) for(int j = 0;j < 3;j++) accel_window[j][i] = 0;
	
	//test I2C
	Wire.begin(9, 5);
	
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(CTRL_REG2);
	Wire.send(1 << RST);
	Wire.endTransmission();

	delay(1000);
	
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(XYZ_DATA_CFG);
	Wire.send(1 << FS1 | 1 << FS0);
	Wire.endTransmission();

	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(CTRL_REG1);
	Wire.send(1 << ACTIVE);
	Wire.endTransmission();
	
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

	SerialUSB.print((x & 0xFF00) >> 8, BYTE);
	SerialUSB.print((x & 0xFF), BYTE);

	SerialUSB.print((y & 0xFF00) >> 8, BYTE);
	SerialUSB.print((y & 0xFF), BYTE);

	SerialUSB.print((z & 0xFF00) >> 8, BYTE);
	SerialUSB.print((z & 0xFF), BYTE);	
}

void loop() {
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(OUT_X_MSB);
	Wire.endTransmission();
	
	Wire.requestFrom(ADDR_DEVICE, 6);

	int16 data[6];

	for(int i = 0;i < 6;i++) {
		uint16 value = (Wire.receive() << 4) | (Wire.receive() >> 4);
		if(value & 0x0800) value |= 0xF000;
		data[i] = value;
	}

	sendXYZ(data[0], data[1], data[2]);	
}

__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while(1) loop();

	return 1;
}
