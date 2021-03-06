#include "controller.h"
#include "color_effects.h"
#include "pixel_effects.h"
#include "accelerometer.h"
#include "wirish.h"

/* Prototypes */

void set_pixel(uint8 pin, boolean state);
void update_pole(void);

/* Defs */

/* POLE MAPPING */
struct Pole { 
 const uint8 color_pins[NUM_SIDES][NUM_RGB];
  const uint8 pixel_pins[NUM_PIXELS];
  uint16 color[NUM_SIDES][NUM_RGB];
  uint8 pixels[NUM_PIXELS];
  ColorEffect * color_effects[MAX_EFFECTS];
  PixelEffect * pixel_effects[MAX_EFFECTS];
};

/* Vars */

Accelerometer accel;

/* COLOR EFFECT INSTANCES */
CLinearFade lfade;
CStatic c_static;
CCircle circle;

/* PIXEL EFFECT INSTANCES */
PChase chase1;
PChase chase2;
PChase chase3;
PStrob strob;
PFullOn full_on; // test

Pole pole = {
  {
    {RED_1, GREEN_1, BLUE_1},
    {RED_2, GREEN_2, BLUE_2},
    {RED_3, GREEN_3, BLUE_3}
  },
  {PIXEL_0, PIXEL_1, PIXEL_2, PIXEL_3, PIXEL_4, PIXEL_5, PIXEL_6, PIXEL_7, PIXEL_8, PIXEL_9, PIXEL_10, PIXEL_11}
};

uint16 tick = 0; // Loop counter

void setup() {
  /* Init Stuff */
  
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    for(uint8 c = 0;c < NUM_RGB;c++) { // Initialize all the Color pins
      pole.color[i][c] = 0x0000; //Struct
      pinMode(pole.color_pins[i][c], PWM); //Pin mode
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]); //Inital PWM value
    }
  }
  
  for(uint8 p = 0;p < NUM_PIXELS;p++) { //Initialize all the pixel pins
    pole.pixels[p] = 0x01; //Struct
#ifdef ZENER
    pinMode(pole.pixel_pins[p], OUTPUT); // Set pins as outputs for Zeners
#endif
    set_pixel(pole.pixel_pins[p], pole.pixels[p]); //Wierd resistor/zener/digitalWrite thing
  }

  for(uint8 m = 0;m < MAX_EFFECTS;m++) { //Initialize the effect arrays
    pole.color_effects[m] = NULL;
    pole.pixel_effects[m] = NULL;
  }

  /* Effects */
  
  /*pole.color_effects[0] = &lfade;
  pole.color_effects[0]->period = 0x0FFF;    
  pole.color_effects[0]->target_colors = {
    {0xFFFF, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000}
  };
  pole.color_effects[0]->status = RUNNING;*/

  pole.color_effects[0] = &c_static;
  pole.color_effects[0]->target_colors = {
    {0xFFFF, 0x0000, 0xFFFF},
    {0xFFFF, 0xFFFF, 0x0000},
    {0x0000, 0xFFFF, 0xFFFF}
  };
  pole.color_effects[0]->blend_mode = BLEND_OR;

  //pole.color_effects[1] = &circle;

  pole.pixel_effects[0] = &full_on; // debug
  /*pole.pixel_effects[0] = &chase1;
  pole.pixel_effects[0]->period = 0x0088;    
  pole.pixel_effects[0]->direction = DIR_UP;
  pole.pixel_effects[0]->offset = 0; // set the offset
  pole.pixel_effects[0]->position = pole.pixel_effects[0]->offset; // make sure the counter is at that offset
  pole.pixel_effects[0]->length = 12;
  pole.pixel_effects[0]->blend_mode = BLEND_OR;*/

  /*pole.pixel_effects[1] = &chase2;
  pole.pixel_effects[1]->period = 0x0087;    
  pole.pixel_effects[1]->direction = DIR_UP;
  pole.pixel_effects[1]->offset = 3;
  pole.pixel_effects[1]->position = pole.pixel_effects[1]->offset; // make sure the counter is at that offset
  pole.pixel_effects[1]->length = 12;
  pole.pixel_effects[1]->blend_mode = BLEND_OR;

  pole.pixel_effects[2] = &chase3;
  pole.pixel_effects[2]->period = 0x0081;    
  pole.pixel_effects[2]->direction = DIR_UP;
  pole.pixel_effects[2]->offset = 6;
  pole.pixel_effects[2]->position = pole.pixel_effects[2]->offset; // make sure the counter is at that offset
  pole.pixel_effects[2]->length = 12;
  pole.pixel_effects[2]->blend_mode = BLEND_OR;*/

  /*pole.pixel_effects[3] = &strob;
  pole.pixel_effects[3]->period = 0x00F8;    
  pole.pixel_effects[3]->blend_mode = BLEND_XOR;*/
}

void loop() {
	accel.pollAndUpdate();
    
  if(accel.mode[0] == 0) {
    pole.color_effects[0]->target_colors = {
      {accel.position[0] & 0xFFFF, 0x0000, 0x0000},
      {accel.position[0] & 0xFFFF, 0x0000, 0x0000},
      {accel.position[0] & 0xFFFF, 0x0000, 0x0000}
    };
  } else if(accel.mode[0] == 1) {
    pole.color_effects[0]->target_colors = {
      {0x0000, accel.position[0] & 0xFFFF, 0x0000},
      {0x0000, accel.position[0] & 0xFFFF, 0x0000},
      {0x0000, accel.position[0] & 0xFFFF, 0x0000}
    };
  } else {
    pole.color_effects[0]->target_colors = {
      {0x0000, 0x0000, accel.position[0] & 0xFFFF},
      {0x0000, 0x0000, accel.position[0] & 0xFFFF},
      {0x0000, 0x0000, accel.position[0] & 0xFFFF}
    };
  }
   
  update_pole();
  
  tick++;
}

void update_pole(void) {
  /* UPDATE COLORS */
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    for(uint8 c = 0;c < NUM_RGB;c++) {
      pole.color[i][c] = 0x0000;
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
      	if(pole.color_effects[m] != NULL) {
      	  pole.color[i][c] = pole.color_effects[m]->update(tick, i, pole.color[i][c], c);
      	}
      }
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }
  }

  /* UPDATE PIXELS */
  for(uint8 p = 0;p < NUM_PIXELS;p++) {
    pole.pixels[p] = 0x00;
    for(uint8 m = 0;m < MAX_EFFECTS;m++) {
      if(pole.pixel_effects[m] != NULL) { // WATCH BLEND MODES AND ORDERS.
      	pole.pixels[p] = pole.pixel_effects[m]->update(tick, pole.pixels[p], p);
      }
    }
    set_pixel(pole.pixel_pins[p], pole.pixels[p]);
  }
}

  
/**
 * Sets a pixel ON or OFF.
 * Depending on the board version it will resort to putting the pin in high impedance to shut the PFET off.
 */
void set_pixel(uint8 pin, uint8 state) {
  if(state) { // ON
#ifdef RESISTOR
    pinMode(pin, OUTPUT);
#endif
    digitalWrite(pin, LOW);
  } else { // OFF
#ifdef RESISTOR
    pinMode(pin, INPUT);
#else
    digitalWrite(pin, HIGH);
#endif
  }
}

__attribute__((constructor)) void premain() {
  init();
}

int main(void) {
  setup();
  
  while (1) {
    loop();
  }
  return 1;
}
