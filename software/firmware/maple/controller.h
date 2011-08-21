#include "wirish.h"

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

// This will affect how the pixels are switched on/off
#define RESISTOR
//#define ZENER

#define CHASE_DOWN 0
#define CHASE_UP 1

#define NUM_SIDES 3
#define NUM_RGB 3
#define NUM_PIXELS 12
#define MAX_EFFECTS 8
#define PIXEL_ON 0x01
#define PIXEL_OFF 0x00

/**
 * I2C
 */
#define ADDR_DEVICE 0x1C

/**
 * RS485
 */
#define RS485 Serial3
#define BAUD_RATE 9600
#define RS485_DIR_PIN 21

/**
 * RGB CHANNELS
 */
#define RED_1 11
#define GREEN_1 12
#define BLUE_1 14

#define RED_2 6
#define GREEN_2 7
#define BLUE_2 8

#define RED_3 1
#define GREEN_3 2
#define BLUE_3 3

/**
 * SECTIONS
 */
#define PIXEL_0 37
#define PIXEL_1 36
#define PIXEL_2 35
#define PIXEL_3 34

#define PIXEL_4 33
#define PIXEL_5 32
#define PIXEL_6 31
#define PIXEL_7 26

#define PIXEL_8 0
#define PIXEL_9 24
#define PIXEL_10 28
#define PIXEL_11 27

/**
 * DEBUG AND EXTRAS
 */
#define BUTTON_1 19
#define BUTTON_2 23
#define BUTTON_3 4

#define STATUS_LED_1 20
#define STATUS_LED_2 22
#define STATUS_LED_3 25

#endif
