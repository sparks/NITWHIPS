#include "color_effects.h"
#include "pixel_effects.h"
#include "controller.h"
#include "wirish.h"

void chaseWhite();
void chaseRGB();
void whiteFace();
void fullWhite();
void whitePixel();
void multiplex();
void off();

struct Pole { 
 const uint8 color_pins[NUM_SIDES][NUM_RGB];
  const uint8 pixel_pins[NUM_PIXELS];
  uint16 color[NUM_SIDES][NUM_RGB];
  uint16 pixels;
  ColorEffect * color_effects[MAX_EFFECTS];
  PixelEffect * pixel_effects[MAX_EFFECTS];
} pole = {
  {
    {RED_1, GREEN_1, BLUE_1},
    {RED_2, GREEN_2, BLUE_2},
    {RED_3, GREEN_3, BLUE_3}
  },
  {
    PIXEL_1_1, PIXEL_1_2, PIXEL_1_3, PIXEL_1_4,
    PIXEL_2_1, PIXEL_2_2, PIXEL_2_3, PIXEL_2_4,
    PIXEL_3_1, PIXEL_3_2, PIXEL_3_3, PIXEL_3_4
  }
};

void set_pixel(uint8 pin, uint8 state) {
  if(state == PIXEL_ON) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  } else if(state == PIXEL_OFF) {
    pinMode(pin, INPUT_PULLUP);
  }
}

uint8 state;

void setup() {
  // Initialize all the pins
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    // Color pins
    for(uint8 c = 0;c < NUM_RGB;c++) {
      pole.color[i][c] = 0x0000;
      pinMode(pole.color_pins[i][c], PWM);
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }
    
    // Pixel pins
  }
  for(uint8 p = 0;p < NUM_PIXELS;p++) {
    set_pixel(pole.pixel_pins[p], PIXEL_OFF);
  }
}

void loop() {
	multiplex();
}

void whitePixel() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		for(uint8 c = 0;c < NUM_RGB;c++) {
			pwmWrite(pole.color_pins[i][c], 0xFFFF); //Turn on all colors this side
		}
	}
	
	for(uint8 p = 0;p < NUM_PIXELS;p++) { //For each pixel
		set_pixel(pole.pixel_pins[(p+NUM_PIXELS-1)%NUM_PIXELS], PIXEL_OFF); //Turn off the last pixel
		set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
		delay(1500); //Pause
	}

	set_pixel(pole.pixel_pins[NUM_PIXELS-1], PIXEL_OFF); //Catch the last pixel
}

void whiteFace() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		for(uint8 c = 0;c < NUM_RGB;c++) {
			pwmWrite(pole.color_pins[(i-1+NUM_SIDES)%NUM_SIDES][c], 0x0000); //Turn off all colors last side
			pwmWrite(pole.color_pins[i][c], 0xFFFF); //Turn on all colors this side
		}

		for(uint8 p = 0;p < NUM_PIXELS;p++) { //For each pixel
			set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
		}
		
		delay(1000); //Pause
	}
}

void fullWhite() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		for(uint8 c = 0;c < NUM_RGB;c++) {
			pwmWrite(pole.color_pins[i][c], 0xFFFF); //Turn on all colors this side
		}
		for(uint8 p = 0;p < NUM_PIXELS;p++) { //For each pixel
			set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
		}
	}
}

void chaseWhite() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		for(uint8 c = 0;c < NUM_RGB;c++) {
			pwmWrite(pole.color_pins[(i-1+NUM_SIDES)%NUM_SIDES][c], 0x0000); //Turn off all colors last side
			pwmWrite(pole.color_pins[i][c], 0xFFFF); //Turn on all colors this side
		}

		for(uint8 p = 0;p < NUM_PIXELS;p++) { //For each pixel
			set_pixel(pole.pixel_pins[(p+NUM_PIXELS-1)%NUM_PIXELS], PIXEL_OFF); //Turn off the last pixel
			set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
			delay(500); //Pause
		}

		set_pixel(pole.pixel_pins[NUM_PIXELS-1], PIXEL_OFF); //Catch the last pixel
	}
}

void multiplex() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		pwmWrite(pole.color_pins[i][0], 0xFFFF); //Turn off the last color
	}
	for(uint8 p = 0;p < 4;p++) {
		set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
	}
	delay(5);
	off();
	
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		pwmWrite(pole.color_pins[i][1], 0xFFFF); //Turn off the last color
	}
	for(uint8 p = 4;p < 8;p++) {
		set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
	}
	delay(5);
	off();
	
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		pwmWrite(pole.color_pins[i][2], 0xFFFF); //Turn off the last color
	}
	for(uint8 p = 8;p < NUM_PIXELS;p++) {
		set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
	}
	delay(5);
	off();
}

void off() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		for(uint8 c = 0;c < NUM_RGB;c++) { //For each color
			pwmWrite(pole.color_pins[i][c], 0x0000); //Turn off
		}
	}
	
	for(uint8 p = 0;p < NUM_PIXELS;p++) { //For each pixel
		set_pixel(pole.pixel_pins[p], PIXEL_OFF); //Turn off
	}
}

void chaseRGB() {
	for(uint8 i = 0;i < NUM_SIDES;i++) { //For each side
		for(uint8 c = 0;c < NUM_RGB;c++) { //For each color
			pwmWrite(pole.color_pins[i][(c+NUM_RGB-1)%NUM_RGB], 0x0000); //Turn off the last color
			pwmWrite(pole.color_pins[i][c], 0xFFFF); //Turn on the current color

			for(uint8 p = 0;p < NUM_PIXELS;p++) {
				set_pixel(pole.pixel_pins[(p+NUM_PIXELS-1)%NUM_PIXELS], PIXEL_OFF); //Turn off the last pixel
				set_pixel(pole.pixel_pins[p], PIXEL_ON); //Turn on the current pixel
				delay(250); //Pause
			}

			set_pixel(pole.pixel_pins[NUM_PIXELS-1], PIXEL_OFF); //Catch the last pixel
		}

		pwmWrite(pole.color_pins[i][NUM_RGB-1], 0x0000); //Catch the last color
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
