#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

/**
 * RS485
 */
#define RS485 Serial3
#define BAUD_RATE 9600
#define RS485_DIR_PIN 21


/**
 * RGB CHANNELS
 * this represents the RGB LEDs on each side of the whip
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
 * this represents the 4 discrete sections of an RGB channel
 * watchout for wiring since the pixel numbering isn't consitent with cat5 pinout.
 */
#define PIXEL_1_1 37
#define PIXEL_1_2 36
#define PIXEL_1_3 35
#define PIXEL_1_4 34

#define PIXEL_2_1 33
#define PIXEL_2_2 32
#define PIXEL_2_3 31
#define PIXEL_2_4 26

#define PIXEL_3_1 0
#define PIXEL_3_2 24
#define PIXEL_3_3 28
#define PIXEL_3_4 27

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
