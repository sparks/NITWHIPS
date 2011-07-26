#include "wirish.h"

#define TEST_PWM_PIN 14

/**
 * FUNCTIONS PROTOTYPES
 */
void print_debug(char);

void setup() {
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(TEST_PWM_PIN, OUTPUT);
  digitalWrite(TEST_PWM_PIN, LOW);
}

void loop() {
  delay(100);
  while(SerialUSB.available()) {
    uint8 incoming = SerialUSB.read();
    print_debug(incoming);
 } 
}

void print_debug(char c) {
  digitalWrite(TEST_PWM_PIN, HIGH);
  SerialUSB.print("MAPLE SAYS >> ");
  SerialUSB.println(c);
  digitalWrite(TEST_PWM_PIN, LOW);
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
