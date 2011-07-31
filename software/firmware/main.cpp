#include "timer.h"
#include "controller.h"

#include "wirish.h"

/*
 * TODO:
 * - setup a timer to update an effect
 * - setup RX
 * - setup TX transmission
 */


void init_rs485(void);
void parse_message(uint8 []);
void transmit_buffer(uint8 [], uint8);

// test
void init_effect(void);
void start_effect(uint8);
void update_effect(void);

uint16 r = 0;
uint16 g = 0;
uint16 b = 0;

uint16 r_step = 0;
uint16 g_step = 0;
uint16 b_step = 0;

uint16 r_target = 0xFFFF;
uint16 g_target = 0x0000;
uint16 b_target = 0xFFFF;

#define BUFFER_SIZE 1
uint8 rx_buffer_index = 0;
uint8 rx_buffer[BUFFER_SIZE];

void setup() {
  init_effect();
  init_rs485();
}

void loop() {
  delay(1);

  if (RS485.available()) {
    uint8 incoming = RS485.read();
    rx_buffer[rx_buffer_index++] = incoming;

    if (rx_buffer_index == BUFFER_SIZE) {
      // Buffer full
      rx_buffer_index = 0; // Reset buffer index
      parse_message(rx_buffer);
    }
  }

  update_effect();
}

void update_effect(void) {
  r += r_step;
  g += g_step;
  b += b_step;

  pwmWrite(RED_1, r);
  pwmWrite(GREEN_1, g);
  pwmWrite(BLUE_1, b);
  
}

void start_effect(uint8 effect_id) {
  r_step = uint16((r_target - r) / 1000);
  g_step = uint16((g_target - g) / 1000);
  b_step = uint16((b_target - b) / 1000);
}
  
  

void parse_message(uint8 buffer[]) {
  uint8 effect_id = (0x03 & *buffer);
  start_effect(effect_id);
}

void transmit_buffer(uint8 buffer[], uint8 buffer_length) {
  uint8 i = 0;

  while(i < buffer_length) {
    digitalWrite(RS485_DIR_PIN, HIGH);
    RS485.write('1');
    i++;
    digitalWrite(RS485_DIR_PIN, LOW);
  }
}

void init_effect(void) {
  pinMode(RED_1, PWM);
  pwmWrite(RED_1, 0);
  pinMode(GREEN_1, PWM);
  pwmWrite(GREEN_1, 0);
  pinMode(BLUE_1, PWM);
  pwmWrite(BLUE_1, 0);
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
