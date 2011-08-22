#include "accelerometer.h"

/* Public */

Accelerometer::Accelerometer() {
  accel_tick = 0;
  
  for(uint8 i = 0;i < AXES;i++) {
    position[i] = 0;
  
    accel[i] = 0;
    accel_lp[i] = 0;
  
    accel_peak_status[i] = PEAK_DEBOUNCE;
    accel_peak_mark[i] = 0;
    accel_local_max[i] = 0;
  
    period[i] = 0;
    mode[i] = 0;
  }
  
  Wire.begin(9, 5);

  accel_write(CTRL_REG2, 1 << RST);

  delay(50);

  while((accel_read(CTRL_REG2) & (1 << RST)) != 0) {
    delay(1);
  }

  accel_write(XYZ_DATA_CFG, 1 << FS1 | 0 << FS0);
  accel_write(CTRL_REG1, 0 << DR2 | 0 << DR1 | 0 << DR0 | 1 << ACTIVE);
}

void Accelerometer::pollAndUpdate() {
  //Read accel data
  uint8 data[AXES*2+1];
  
  for(uint8 i = 0;i < AXES*2+1;i++) data[i] = 0;
  accel_multi_read(STATUS, AXES*2+1, data);
  
  if((data[0] & ((1 << XDR) | (1 << YDR))) == ((1 << XDR) | (1 << YDR))) { //Wait for new data
    //Parse into signed ints
    for(uint8 i = 0;i < AXES;i++) accel[i] = 0;

    for(uint8 i = 0;i < AXES;i++) {
      accel[i] = (data[i*2+1] << 4) | (data[i*2+2] >> 4);
      if(accel[i] & 0x0800) accel[i] |= 0xF000;
      accel_lp[i] = (int16)(ACCEL_ALPHA*accel[i]+(1-ACCEL_ALPHA)*accel_lp[i]);
    }

    computePeriod();
    computeMode();
    computePosition();
          
    accel_tick++;
  }
}

void Accelerometer::sendAccel() {
  mark();

  for(uint8 i = 0;i < AXES;i++) {
    SerialUSB.print((accel[i] & 0xFF00) >> 8, BYTE);
    SerialUSB.print((accel[i] & 0xFF), BYTE);
  }
}

void Accelerometer::sendAccelLP() {
  mark();

  for(uint8 i = 0;i < AXES;i++) {
    SerialUSB.print((accel_lp[i] & 0xFF00) >> 8, BYTE);
    SerialUSB.print((accel_lp[i] & 0xFF), BYTE);
  }
}


void Accelerometer::sendPeriod() {
  mark();
  
  for(uint8 i = 0;i < AXES;i++) {
    SerialUSB.print((period[i] & 0xFF00) >> 8, BYTE);
    SerialUSB.print((period[i] & 0xFF), BYTE);
  }
}

void Accelerometer::sendPosition() {
  mark();
  
  for(uint8 i = 0;i < AXES;i++) {
    SerialUSB.print((position[i] & 0xFF00) >> 8, BYTE);
    SerialUSB.print((position[i] & 0xFF), BYTE);
  }
}


void Accelerometer::whoAmI() {
  uint8 res = accel_read(WHO_AM_I);
  SerialUSB.println(res, HEX);
}

void Accelerometer::printXYZ() {
  SerialUSB.print(accel[0], DEC);
  SerialUSB.print("\t");
  SerialUSB.print(accel[1], DEC);
  SerialUSB.print("\t");
  SerialUSB.println(accel[2], DEC);
}

/* Private */

void Accelerometer::computePeriod() {
  for(uint8 i = 0;i < AXES;i++) {     
    if(accel_lp[i] > accel_local_max[i]) accel_local_max[i] = abs(accel_lp[i]);
    
    if(accel_local_max[i] > ACTIVITY_THRES) {
      int new_period = 0;
      
      if(accel_lp[i] > 0.75*accel_local_max[i] && accel_peak_status[i] == 0) {
        accel_peak_status[i] = PEAK_DEBOUNCE;
        
        if(accel_peak_mark[i] == 0) accel_peak_mark[i] = accel_tick;
        else {
          if(accel_tick > accel_peak_mark[i]) new_period = accel_tick-accel_peak_mark[i];
          accel_peak_mark[i] = 0;
        }
      } else if(accel_lp[i] < 0.75*accel_local_max[i] && accel_peak_status[i] > 0) {
        accel_peak_status[i]--;
      }
      
      if(accel_tick > accel_peak_mark[i] && accel_peak_mark[i] != 0 && accel_tick-accel_peak_mark[i] > MAX_PERIOD ) {
        period[i] = period[i]*(1-PERIOD_ALPHA);
        accel_local_max[i] = 0;

        accel_peak_status[i] = PEAK_DEBOUNCE;
        accel_peak_mark[i] = 0;
      }

      if(new_period != 0 && new_period < MAX_PERIOD) period[i] = (int16)(PERIOD_ALPHA*new_period+(1-PERIOD_ALPHA)*period[i]);
    } else {
      period[i] = period[i]*(1-PERIOD_ALPHA);
    }
    
    accel_local_max[i]--;
  }
}

void Accelerometer::computeMode() { 
  for(uint8 i = 0;i < AXES;i++) {     
    if(period[i] < MODE1) mode[i] = 0;
    if(period[i] < MODE2) mode[i] = 1;
    if(period[i] < MODE3) mode[i] = 2;
    else mode[i] = 3;
  }
  
  //Some sort of variance analysis needed for a noise mode
}

void Accelerometer::computePosition() {
  for(uint8 i = 0;i < AXES;i++) position[i] = map(constrain(-accel_lp[i], -accel_local_max[i], accel_local_max[i]), -accel_local_max[i], accel_local_max[i], 0x0000, 0xFFFF);
}

uint8 Accelerometer::accel_write(uint8 addr, uint8 data) {
  Wire.beginTransmission(ADDR_DEVICE);
  Wire.send(addr);
  Wire.send(data);
  return Wire.endTransmission();
}

uint8 Accelerometer::accel_read(uint8 addr) {
  Wire.requestFrom(ADDR_DEVICE, 1);
  return Wire.receive();
}

uint8 Accelerometer::accel_multi_read(uint8 addr, uint8 num, uint8 data[]) {
  Wire.beginTransmission(ADDR_DEVICE);
  Wire.send(addr);
  Wire.endTransmission();

  uint8 rx_len = Wire.requestFrom(ADDR_DEVICE, num);
  for(uint8 i = 0;i < num;i++) data[i] = Wire.receive();
  return rx_len;
}

void Accelerometer::mark() {
  for(uint8 i = 0;i < MARK_LEN;i++) SerialUSB.print(0xFF, BYTE);
}
