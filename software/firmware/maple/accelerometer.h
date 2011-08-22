#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <stdio.h>

#include "wirish.h"
#include "Wire.h"

#include "MMA8452Q.h"

#define ADDR_DEVICE 0x1C

#define AXES 2
#define MARK_LEN 2

#define ACCEL_ALPHA 0.2

#define ACTIVITY_THRES 500

#define MAX_PERIOD 300
#define PERIOD_ALPHA 0.5
#define PEAK_DEBOUNCE 5

#define MODE1 50
#define MODE2 100
#define MODE3 250

class Accelerometer {
  public:
    uint16 position[AXES]; //The current position estimate for the poll

    int16 accel[AXES]; //Latest raw accelerometer data
    int16 accel_lp[AXES]; //Latest accelerometer data through a first order digital low pass

    int16 period[AXES]; //The current period estimates
    int8 mode[AXES];
  
    Accelerometer();
  
    void pollAndUpdate(); //Run all the relevant methods to poll the accelerometer and update the estimates

    /* Data transfer to Processing */
    void sendAccel(); //Send latest accelerometer data
    void sendAccelLP(); //Send latest low passed accelerometer data
    void sendPeriod(); //Send latest period data 
    void sendPosition(); //Send latest period data 
    
    /* Actuall numerical printouts */
    void whoAmI(); //Print the WHO_AM_I value to USBSerial
    void printXYZ(); //Print the XYZ to USBSerial

  private:
    uint accel_tick; //Counts the frames received
    
    int16 accel_peak_status[AXES]; //Status and debounce variable for the peak tracker
    uint accel_peak_mark[AXES]; //Mark variable to measure samples between peaks
    
    int16 accel_local_max[AXES]; //Most recent maximum value
    
    void computePeriod(); //Run peak to peak calculations on the most recent frame
    void computeMode(); //Compute mode based on the period
    void computePosition(); //Compute the position based on acceleration and mode
    
    /* Accelerometer communications */
    uint8 accel_write(uint8 addr, uint8 data); //Write one byte
    uint8 accel_read(uint8 addr); //Read one byte
    uint8 accel_multi_read(uint8 addr, uint8 num, uint8 data[]); //Read multiple bytes

    /* Data transfer to Processing */
    void mark(); //Send a processing mark (MARK_LEN bytes of 0xFF)
};

#endif
