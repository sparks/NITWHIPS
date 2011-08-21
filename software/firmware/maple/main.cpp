#include "wirish.h"
#include "Wire.h"

#include "MMA8452Q.h"

#define ADDR_DEVICE 0x1C
#define I2C_WRITE 0
#define I2C_READ  1

#define WINDOW_SIZE 128

int16 accel_window[3][WINDOW_SIZE];
int16 fft_data[3][WINDOW_SIZE];

void who_am_i();
uint8 accel_write(uint8 addr, uint8 data);
boolean accel_read(uint8 addr, uint8* data);
boolean accel_multi_read(uint8 addr, uint8 num, uint8 data[]);
void printXYZ(int x, int y, int z);
void sendXYZ(int x, int y, int z);
void loop();

Port port;

void setup() {
	
	for(int i = 0;i < WINDOW_SIZE;i++) for(int j = 0;j < 3;j++) accel_window[j][i] = 0;
	   
	Wire.begin(9, 5);
	// port.sda = 9;
	// port.scl = 5;
	// 
	// pinMode(port.scl, OUTPUT_OPEN_DRAIN);
	//     pinMode(port.sda, OUTPUT_OPEN_DRAIN);
	//     digitalWrite(port.scl, HIGH);
	//     digitalWrite(port.sda, HIGH);
	
	accel_write(CTRL_REG2, 1 << RST);
	
	// uint8 result = 1 << RST;
	// 
	// while((result & (1 << RST)) != 0) {
	// 	accel_read(CTRL_REG2, &result);
	// 	SerialUSB.println("Waiting for accel reset");
	// 	delay(500);
	// }

	delay(1000);
	
	accel_write(XYZ_DATA_CFG, 1 << FS1 | 1 << FS0);
	accel_write(CTRL_REG1, 1 << ACTIVE);	
}

uint8 accel_write(uint8 addr, uint8 data) {
	// i2c_start(port);
	// 
	// i2c_shift_out(port, (ADDR_DEVICE << 1) | I2C_WRITE);
	// if(!i2c_get_ack(port)) return false;
	//     
	// i2c_shift_out(port, addr);
	// if(!i2c_get_ack(port)) return false;
	// 
	//     i2c_shift_out(port, data);
	// if(!i2c_get_ack(port)) return false;
	// 
	// i2c_stop(port);
	// 
	// return true;
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(addr);
	Wire.send(data);
	return Wire.endTransmission();
}

void who_am_i() {
	uint8 res = 0;
	accel_read(WHO_AM_I, &res);
	SerialUSB.println(res, HEX);
	delay(500);
}

boolean accel_read(uint8 addr, uint8 *data) {
	uint8 rx_len = Wire.requestFrom(ADDR_DEVICE, 1);
	*data = Wire.receive();
	return rx_len;
	
	// i2c_start(port);
	// 
	//     i2c_shift_out(port, (ADDR_DEVICE << 1) | I2C_WRITE);
	// if(!i2c_get_ack(port)) return false;
	// 
	// i2c_shift_out(port, addr);
	// if(!i2c_get_ack(port)) return false;
	// 
	// i2c_stop(port);
	// i2c_start(port);
	// 
	//     i2c_shift_out(port, (ADDR_DEVICE << 1) | I2C_READ);
	// if(!i2c_get_ack(port)) return false;
	// 
	// *data = i2c_shift_in(port);
	// i2c_send_nack(port);
	// 
	// i2c_stop(port);
	// 
	// return true;
}

boolean accel_multi_read(uint8 addr, uint8 num, uint8 data[]) {
	Wire.beginTransmission(ADDR_DEVICE);
	Wire.send(addr);
	Wire.endTransmission();
	
	uint8 rx_len = Wire.requestFrom(ADDR_DEVICE, num);
	for(int i = 0;i < num;i++) data[i] = Wire.receive();
	return rx_len;
	
	// 
	// i2c_start(port);
	// 
	//     i2c_shift_out(port, (ADDR_DEVICE << 1) | I2C_WRITE);
	// if(!i2c_get_ack(port)) return false;
	// 
	// i2c_shift_out(port, addr);
	// if(!i2c_get_ack(port)) return false;
	// 
	// i2c_stop(port);
	// i2c_start(port);
	// 
	//     i2c_shift_out(port, (ADDR_DEVICE << 1) | I2C_READ);
	// if(!i2c_get_ack(port)) return false;
	// 
	// for(int i = 0;i < num;i++) {
	// 	data[i] = i2c_shift_in(port);
	// 	if(i < num-1) i2c_send_ack(port);
	// 	else i2c_send_nack(port);
	// }
	// 
	// i2c_stop(port);
	// 
	// return true;
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

void loop() {
	// uint8 data = 0;
	// accel_read(OUT_X_MSB, &data);
	// SerialUSB.println(data);
	// delay(100);
	
	uint8 data[6] = {0, 0, 0, 0, 0, 0};
	accel_multi_read(OUT_X_MSB, 6, data);
		
	int16 accel_values[3] = {0, 0, 0};
	
	for(int i = 0;i < 3;i++) {
		accel_values[i] = (data[i*2] << 4) | (data[i*2+1] >> 4);
		if(accel_values[i] & 0x0800) accel_values[i] |= 0xF000;
	}
	
	printXYZ(accel_values[0], accel_values[1], accel_values[2]);
}

__attribute__((constructor)) void premain() {
	init();
}

int main(void) {
	setup();

	while(1) loop();

	return 1;
}
