#include "controller.h"
#include "wirish.h"

uint16 tick = 0;


class ColorEffect {
public:
  uint16 update(uint16 tick, uint16 sides, uint16 color);
};

uint16 ColorEffect::update(uint16 tick, uint16 sides, uint16 color) {
  return tick % 0xFFFF;
}

#define NUM_SIDES 1
#define NUM_RGB 3
#define NUM_PIXELS 4
#define MAX_EFFECTS 8
uint8 colors[NUM_SIDES][NUM_RGB];
uint8 pixels[NUM_SIDES];
ColorEffect effects_color[NUM_SIDES][NUM_RGB][MAX_EFFECTS];

#define BUFFER_SIZE 1
uint8 rx_buffer_index = 0;
uint8 rx_buffer[BUFFER_SIZE];
void init_rs485(void);

void setup() {
  init_rs485();
  colors[0] = { RED_1, GREEN_1, BLUE_1 };

  pinMode(RED_1, PWM);
  pinMode(GREEN_1, PWM);
  pinMode(BLUE_1, PWM);

  for(uint8 i = 0;i < NUM_SIDES;i++) {
    for(int8 c = 0;c < NUM_RGB;c++) {
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	pwmWrite(colors[i][c], 0x0000);
      }
    }
  }
}

void loop() {
  delay(1);

  // Receive data
  if (RS485.available()) {
    uint8 incoming = RS485.read();
    rx_buffer[rx_buffer_index++] = incoming;
    if (rx_buffer_index == BUFFER_SIZE) {
      // Buffer full
      rx_buffer_index = 0; // Reset buffer indexx
    }
  }
  
  for(uint8 i = 0;i < NUM_SIDES;i++) {
    for(uint8 c = 0;c < NUM_RGB;c++) {
      uint16 tmp = 0xFFFF;
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	tmp = tmp*effects_color[i][c][m].update(tick, i, c)/0xFFFF;
      }
      pwmWrite(colors[i][c], tmp);
   }
      
      
    pixels[i] = 0x00;
    for(uint8 p = 0;p < NUM_PIXELS;p++) {
      pixels[i] |= 1 << p;
    }
    
  }
  
  tick++;
}


void init_rs485(void) {
  RS485.begin(BAUD_RATE);

  // Transeiver directional pin setup.  Default low, receiver enabled.
  pinMode(RS485_DIR_PIN, OUTPUT);
  digitalWrite(RS485_DIR_PIN, LOW);
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
