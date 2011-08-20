#include "controller.h"
#include "color_effects.h"
#include "pixel_effects.h"
#include "MMA8452Q.h"

#include "Wire.h"
#include "wirish.h"

#define RX_BUFFER_SIZE 1
uint8 rx_buffer_index = 0;
uint8 rx_buffer[RX_BUFFER_SIZE];

uint16 tick = 0;

void set_pixel(uint8, uint8);
void transmit_byte(char b);

/**
 * EFFECT INSTANCES
 */
LFade lfade(0xFFFF);
StrobChase strob_chase(0x0F00);
Strob strob(0x000F);

/**
 * POLE MAPPING
 */
struct Pole { 
 const uint8 color_pins[NUM_SIDES][NUM_RGB];
  const uint8 pixel_pins[NUM_PIXELS];
  uint16 color[NUM_SIDES][NUM_RGB];
  boolean pixels[NUM_PIXELS];
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

  //I2C
  Wire.begin(9, 5);

  // Init RS485
  Serial3.begin(9600);
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
    for(uint8 p = 0;p < NUM_PIXELS;p++) {
      if((1 << p) & pole.pixels[i]) {
	set_pixel(pole.pixel_pins[p], PIXEL_ON);
      }
      else {
	set_pixel(pole.pixel_pins[p], PIXEL_OFF);
      }
    }
  }

  for(uint8 m = 0;m < MAX_EFFECTS;m++) {
    pole.color_effects[m] = NULL;
  }

  // ADD EFFECTS
  pole.color_effects[0] = &lfade;
  //pole.pixel_effects[0] = &strob_chase;
  //pole.pixel_effects[0] = &strob;

  
}

void loop() {
  delay(10);

  if(Serial3.available()) {
    char incoming = Serial3.read();
    digitalWrite(RS485_DIR_PIN, HIGH);
    Serial3.println(incoming);
    digitalWrite(RS485_DIR_PIN, LOW);

    pole.color_effects[0]->period = (incoming * 8) & 0xFFFF;
    pole.pixel_effects[0]->period = incoming;
  }

  // Receive data
  if (SerialUSB.available()) {
    uint8 incoming = SerialUSB.read();

    /*Wire.beginTransmission(ADDR_DEVICE);
    Wire.send(OUT_X_MSB);
    Wire.endTransmission();
    Wire.requestFrom(ADDR_DEVICE, 2);
    while(!Wire.available());
    int value;
    value = (Wire.receive() << 4);
    if(value & 0x800) value |= uint32(0xFFFFF000);
    value |= Wire.receive();*/

    /*uint16 value;
      value = Wire.receive() << 4 | Wire.receive() >> 4;*/
    // SerialUSB.println(value);


    /*rx_buffer[rx_buffer_index++] = incoming;
        if (rx_buffer_index == BUFFER_SIZE) {
      // Buffer full
      rx_buffer_index = 0; // Reset buffer indexx
      }*/
  }


  for(uint8 i = 0;i < NUM_SIDES;i++) {

    for(uint8 c = 0;c < NUM_RGB;c++) {
      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	if(pole.color_effects[m] != NULL) {
	  //pole.color[i][c] = pole.color_effects[m]->update(tick, i, c);
	}
      }
      pwmWrite(pole.color_pins[i][c], pole.color[i][c]);
    }

    
    for(uint8 p = 0;p < NUM_PIXELS;p++) {
      //SerialUSB.println(pole.pixel_effects[0]->update(tick, i, pole.pixels[i], p));

      /*for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	if(pole.pixel_effects[m] != NULL) {
	  pole.pixels[i] ^= (pole.pixel_effects[m]->update(tick, i, pole.pixels[i], p) << p);
	}
	}*/
    
      if((1 << p) & pole.pixels[i]) {
	set_pixel(pole.pixel_pins[p], PIXEL_ON);
      }
      else {
	set_pixel(pole.pixel_pins[p], PIXEL_OFF);
      }
    }
    
  }
  tick++;
}

/**
 * Sets a pixel ON or OFF.
 * Depending on the board version it will resort to putting the pin in high impedance to shut the PFET off.
 */
void set_pixel(uint8 pin, uint8 state) {
  if(state == PIXEL_ON) {
#ifdef RESISTOR
    pinMode(pin, OUTPUT);
#endif
    digitalWrite(pin, LOW);

  } else if(state == PIXEL_OFF) {
#ifdef RESISTOR
    pinMode(pin, INPUT);
#elif ZENER
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
