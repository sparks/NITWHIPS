#include "controller.h"
#include "wirish.h"

/*
 * TODO:
 * - setup a timer to update an effect
 * - setup RX
 * - setup TX transmission
 */


class LEDStrip {
private:
  uint8 red_pin, green_pin, blue_pin;
  uint8 pixels[4];
  uint8 pixel_status;
  uint8 status;
public:
  uint16 r, g, b;
  void init_strip(uint8, uint8, uint8, uint8, uint8, uint8, uint8);
  void set_color(uint16, uint16, uint16);
  void blackout(void);
  void set_pixels(uint8);
} strip;

void LEDStrip::init_strip(uint8 rp, uint8 gp, uint8 bp, uint8 p1, uint8 p2, uint8 p3, uint8 p4) {
  red_pin = rp;
  green_pin = gp;
  blue_pin = bp;

  pinMode(red_pin, PWM);
  pinMode(green_pin, PWM);
  pinMode(blue_pin, PWM);

  pixels[0] = p1;
  pixels[1] = p2;
  pixels[2] = p3;
  pixels[3] = p4;
  pinMode(pixels[0], OUTPUT);
  pinMode(pixels[1], OUTPUT);
  pinMode(pixels[2], OUTPUT);
  pinMode(pixels[3], OUTPUT);

  blackout();
}

void LEDStrip::set_pixels(uint8 mapping) {
  pixel_status = mapping;
  uint8 j = 0;
  for(uint8 i = 0x01;i < 0x10;i <<= 1) {
    digitalWrite(j++, mapping & i);
  }
}

void LEDStrip::set_color(uint16 r, uint16 g, uint16 b) {
  r = r;
  g = g;
  b = b;
  pwmWrite(red_pin, r);
  pwmWrite(green_pin, g);
  pwmWrite(blue_pin, b);
}

void LEDStrip::blackout(void) {
  set_color(0, 0, 0);
}

LEDStrip strip_1;

class Effect {
  #define RUNNING 0
  #define DONE 1
private:
  uint16 target_red, target_green, target_blue;
  LEDStrip strp;
public:
  uint16 time, tick;
  uint8 status;
  void start_effect(LEDStrip, uint16, uint16, uint16, uint16);
  void effect_done(void);
  void update(void);
} effect;

void Effect::start_effect(LEDStrip strp, uint16 rt, uint16 gt, uint16 bt, uint16 tt) {
  target_red = rt;
  target_green = gt;
  target_blue = bt;
  time = tt;
  strp = strp;
  status = RUNNING;
}

void Effect::effect_done(void) {
  status = DONE;
}

void Effect::update(void) {
  //strp.r += int((target_red - strp.r) / tick);
  strp.set_color(0xFFFF, 0, 0);
    //tick++;
}

Effect efx;
  
#define BUFFER_SIZE 1
uint8 rx_buffer_index = 0;
uint8 rx_buffer[BUFFER_SIZE];

void init_rs485(void);
void parse_message(uint8 []);
void transmit_buffer(uint8 [], uint8);

void setup() {
  init_rs485();
  strip_1.init_strip(RED_1, GREEN_1, BLUE_1, PIXEL_1_1, PIXEL_1_2, PIXEL_1_3, PIXEL_1_4); // LOL
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

  efx.update();
    
}



void parse_message(uint8 buffer[]) {
  efx.start_effect(strip_1, 0xFFFF, 0, 0, 1000);
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
