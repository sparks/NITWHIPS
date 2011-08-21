#include "controller.h"
#include "color_effects.h"
#include "pixel_effects.h"
#include "wirish.h"

uint16 tick = 0; // Loop counter

void set_pixel(uint8 pin, boolean state);

/**
 * COLOR EFFECT INSTANCES
 */
CLinearFade lfade(0xFFFF);
CCircle circle(0xFFFF);

/**
 * PIXEL EFFECT INSTANCES
 */
PChase chase(0x00FF);
PStrob strob(0x000F);
//PRing ring();

/**
 * POLE MAPPING
 */
struct Pole { 
 const uint8 color_pins[NUM_SIDES][NUM_RGB];
  const uint8 pixel_pins[NUM_PIXELS];
  uint16 color[NUM_SIDES][NUM_RGB];
  uint8 pixels[NUM_PIXELS];
  ColorEffect * color_effects[MAX_EFFECTS];
  PixelEffect * pixel_effects[MAX_EFFECTS];
} pole = {
  {
    {RED_1, GREEN_1, BLUE_1},
    {RED_2, GREEN_2, BLUE_2},
    {RED_3, GREEN_3, BLUE_3}
  },
  {PIXEL_0, PIXEL_1, PIXEL_2, PIXEL_3, PIXEL_4, PIXEL_5, PIXEL_6, PIXEL_7, PIXEL_8, PIXEL_9, PIXEL_10, PIXEL_11}
};


void setup() {
  //debug led
  pinMode(25, OUTPUT);

  // Initialize all the pins
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    // Color pins
    for(uint8 c = 0;c < NUM_RGB;c++) {
      pole.color[i][c] = 0xFF00;
      pinMode(pole.color_pins[i][c], PWM);
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }
    for(uint8 p = 0;p < NUM_PIXELS;p++) {
      pole.pixels[p] = 0x01;
      set_pixel(pole.pixel_pins[p], pole.pixels[p]);
    }
  }

  for(uint8 m = 0;m < MAX_EFFECTS;m++) {
    pole.color_effects[m] = NULL;
    pole.pixel_effects[m] = NULL;
  }

  // ADD EFFECTS
  pole.color_effects[0] = &lfade;
  pole.color_effects[0]->target_colors = {
    {0xFFFF, 0x0000, 0x0000},
    {0x0000, 0xFFFF, 0x0000},
    {0x0000, 0x0000, 0xFFFF}
    };

  pole.color_effects[1] = &circle;
  circle.direction = DIR_DOWN;

  pole.pixel_effects[0] = &strob;
  pole.pixel_effects[1] = &chase;
  chase.direction = DIR_DOWN;
}

void loop() {
  delay(1);

  // UPDATE COLORS
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    for(uint8 c = 0;c < NUM_RGB;c++) {
      pole.color[i][c] = 0xFFFF;
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	if(pole.color_effects[m] != NULL) {
	  pole.color[i][c] &= pole.color_effects[m]->update(tick, i, c);
	}
      }
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }
  }

  //UPDATE PIXELS
  for(uint8 p = 0;p < NUM_PIXELS;p++) {
    pole.pixels[p] = 0x01;
    for(uint8 m = 0;m < MAX_EFFECTS;m++) {
      if(pole.pixel_effects[m] != NULL) {
	pole.pixels[p] &= pole.pixel_effects[m]->update(tick, pole.pixels[p], p);
      }
    }
    set_pixel(pole.pixel_pins[p], pole.pixels[p]);
  }
  
  tick++;
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
