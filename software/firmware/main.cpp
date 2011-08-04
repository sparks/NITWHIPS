#include "controller.h"
#include "wirish.h"

#define NUM_SIDES 3
#define NUM_RGB 3
#define NUM_PIXELS 4
#define MAX_EFFECTS 8
#define PIXEL_ON 0x01
#define PIXEL_OFF 0x00

class PixelEffect {
public:
  uint16 period;
  PixelEffect(uint16 p) {
    period = p;
  };
  virtual uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) {
    return (0x00);
  }
};

class Strob: public PixelEffect {
public:
  Strob(uint16 p): PixelEffect(p) {
    period = p;
  };
  uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) {
    if((tick % (period/2)) == 0) {
      pixel ^= (1 << pixel_index);;
    }
    return pixel;
  };
};

class StrobChase: public PixelEffect {
public:
  uint8 chase_position;
  uint8 chase_length;
 
  StrobChase(uint16 p): PixelEffect(p) {
    period = p;
    chase_position = 0;
    chase_length = 2;
  };
  uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) {
    if((pixel_index == 0) && (tick % period == 0)) {
      //pixel ^= 0x0F;
      if(pixel >= (1 << (chase_length - 1))) pixel = 0x01;
      else pixel <<= 1;
    }
    return pixel;
  };
};
 
class ColorEffect {
public:
  uint16 target_color, period;
  virtual uint16 update(uint16 tick, uint16 side, uint16 channel) {
    return (0x0000);
  }
};

class LFade: public ColorEffect {
public:
  uint16 update(uint16 tick, uint16 side, uint16 channel) {
      return (tick % 0xFFFF);
  }
};

struct Pole { 
 const uint8 color_pins[NUM_SIDES][NUM_RGB];
  const uint8 pixel_pins[NUM_SIDES][NUM_PIXELS];
  uint16 color[NUM_SIDES][NUM_RGB];
  uint8 pixels[NUM_SIDES];
  ColorEffect * color_effects[MAX_EFFECTS];
  PixelEffect * pixel_effects[MAX_EFFECTS];
} pole = {
  {
    {RED_1, GREEN_1, BLUE_1},
    {RED_2, GREEN_2, BLUE_2},
    {RED_3, GREEN_3, BLUE_3}
  },
  {
    {PIXEL_1_1, PIXEL_1_2, PIXEL_1_3, PIXEL_1_4},
    {PIXEL_2_1, PIXEL_2_2, PIXEL_2_3, PIXEL_2_4},
    {PIXEL_3_1, PIXEL_3_2, PIXEL_3_3, PIXEL_3_4}
  }
};

void set_pixel(uint8 pin, uint8 state) {
  if(state == PIXEL_ON) {
    pinMode(pin, OUTPUT);
  } else if(state == PIXEL_OFF) {
    pinMode(pin, INPUT);
  }
}

#define BUFFER_SIZE 1
uint8 rx_buffer_index = 0;
uint8 rx_buffer[BUFFER_SIZE];
void transmit_byte(uint8 b);

uint16 tick = 0;

LFade lfade;
StrobChase strobber(0x0100);

void setup() {
  // Init RS485
  Serial3.begin(BAUD_RATE);
  // Transeiver directional pin setup.  Default low, receiver enabled.
  pinMode(RS485_DIR_PIN, OUTPUT);
  digitalWrite(RS485_DIR_PIN, LOW);

  // Initialize all the pins
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    // Color pins
    for(uint8 c = 0;c < NUM_RGB;c++) {
      pole.color[i][c] = 0xFFFF;
      pinMode(pole.color_pins[i][c], PWM);
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }
    // Pixel pins
    pole.pixels[i] = 0x0F;
    for(uint8 p = 0;p < NUM_PIXELS;p++) {
      if((1 << p) & pole.pixels[i]) {
	set_pixel(pole.pixel_pins[i][p], PIXEL_ON);
      }
      else {
	set_pixel(pole.pixel_pins[i][p], PIXEL_OFF);
      }
    }
  }

  for(uint8 m = 0;m < MAX_EFFECTS;m++) {
    pole.color_effects[m] = NULL;
  }

  pole.color_effects[0] = &lfade;
  pole.pixel_effects[0] = &strobber;

}

void loop() {
  delay(1);

  // debug
  if(SerialUSB.available()) {
    char incoming = SerialUSB.read();
    pole.pixel_effects[0]->period = incoming;
  }

  // Receive data
  /*if (Serial3.available()) {
    uint8 incoming = Serial3.read();
    rx_buffer[rx_buffer_index++] = incoming;
        
    if (rx_buffer_index == BUFFER_SIZE) {
      // Buffer full
      rx_buffer_index = 0; // Reset buffer indexx
      }
  }*/

  for(uint8 i = 0;i < NUM_SIDES;i++) {

    for(uint8 c = 0;c < NUM_RGB;c++) {
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	if(pole.color_effects[m] != NULL) {
	  pole.color[i][c] = pole.color_effects[m]->update(tick, i, c);
	}
      }
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }

    for(uint8 p = 0;p < NUM_PIXELS;p++) {
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	if(pole.pixel_effects[m] != NULL) {
	  pole.pixels[i] = pole.pixel_effects[m]->update(tick, i, pole.pixels[i], p);
	}
      }
    
      if((1 << p) & pole.pixels[i]) {
	set_pixel(pole.pixel_pins[i][p], PIXEL_ON);
      }
      else {
	set_pixel(pole.pixel_pins[i][p], PIXEL_OFF);
      }
    }
    
  }

  
  tick++;
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
