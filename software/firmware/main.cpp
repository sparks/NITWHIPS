#include "wirish.h"

#define BAUD_RATE 9600
#define DIR_PIN 21

#define TEST_RED_CH 11
#define TEST_GREEN_CH 12
#define TEST_BLUE_CH 14

/**
 * FUNCTIONS PROTOTYPES
 */
void print_debug(char c);
void rs485_transmit(char c);
uint16 r, g, b = 0x01;

void setup() {
  // debub led
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, LOW);

  digitalWrite(DIR_PIN, LOW); // RX enable
  Serial3.begin(BAUD_RATE);

  pinMode(TEST_RED_CH, PWM);
  pwmWrite(TEST_RED_CH, 1000);

  pinMode(TEST_GREEN_CH, PWM);
  pwmWrite(TEST_GREEN_CH, 20000);

  pinMode(TEST_BLUE_CH, PWM);
  pwmWrite(TEST_BLUE_CH, 20000);

}

void loop() {
  delay(1);
  
  while(Serial3.available()) {
    uint8 incoming = Serial3.read();
    print_debug(incoming);
    rs485_transmit(incoming);
  }
}

void rs485_transmit(char c) {
  digitalWrite(DIR_PIN, HIGH); // TX enable
  Serial3.println(c);
  digitalWrite(DIR_PIN, LOW); // RX enable
}

void print_debug(char c) {
  digitalWrite(BOARD_LED_PIN, HIGH);
  SerialUSB.print("MAPLE SAYS >> ");
  SerialUSB.println(c);
  digitalWrite(BOARD_LED_PIN, LOW);
}


// -- premain() and main() ----------------------------------------------------
// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
  init();
}

int main(void) {
  setup();
  
  while (1) {
    loop();
  }
  return 0;
}
