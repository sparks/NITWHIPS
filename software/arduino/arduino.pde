#include "MMA8452Q.h"
#include <Wire.h>

#define ADDR_DEVICE 0x1C

void setup() {
	Serial.begin(115200);
		
	Wire.begin();

	Wire.beginTransmission(ADDR_DEVICE);

	Wire.send(CTRL_REG1);
	Wire.send(1 << ACTIVE | 1 << F_READ);

	Wire.endTransmission();
}

void loop() {
	Wire.beginTransmission(ADDR_DEVICE);
	
	Wire.send(OUT_X_MSB);
	
	Wire.endTransmission();

	Wire.requestFrom(ADDR_DEVICE, 2);
	
	while(Wire.available() < 2);

	int data;
	
	data = (Wire.receive() << 4);
	if(data & 0x800) data |= uint16_t(0xFFFFF000);
	data |= Wire.receive();
	
	Serial.println(data, DEC);
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