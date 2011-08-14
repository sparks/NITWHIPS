#include "MMA8452Q.h"
#include <Wire.h>

#define ADDR_DEVICE 0x1C

void setup() {
	Serial.begin(115200);
	
	Wire.begin();

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

void loop() {	
	Wire.requestFrom(ADDR_DEVICE, 7);
	
	byte status = Wire.receive();
		
	int16_t data[6];
	
	for(int i = 0;i < 6;i++) {
		uint16_t value = (Wire.receive() << 4) | (Wire.receive() >> 4);
		if(value & 0x0800) value |= 0xF000;
		data[i] = value;
	}
	
	sendXYZ(data[0], data[1], data[2]);
}


void sendXYZ(int x, int y, int z) {
	Serial.print(0xFF, BYTE);
	Serial.print(0xFF, BYTE);

	Serial.print((x & 0xFF00) >> 8, BYTE);
	Serial.print((x & 0xFF), BYTE);

	Serial.print((y & 0xFF00) >> 8, BYTE);
	Serial.print((y & 0xFF), BYTE);

	Serial.print((z & 0xFF00) >> 8, BYTE);
	Serial.print((z & 0xFF), BYTE);	
}