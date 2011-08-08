#include "controller.h"
#include "color_effects.h"
#include "pixel_effects.h"
//#include "MMA8452Q.h"
#include "wirish.h"

#include "Wire.h"

// test i2c
#define ADDR_DEVICE 0x1C

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
void transmit_byte(char b);

uint16 tick = 0;

LFade lfade(0xFFFF);
StrobChase strob_chase(0x0F00);
Strob strob(0x000F);


void transmit_byte(char b) {
}

void setup() {

  //debug
  pinMode(25, OUTPUT);

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
  pole.pixel_effects[0] = &strob_chase;
  //pole.pixel_effects[1] = &strob;

  //test I2C
  /*Wire.begin(9, 5);
  Wire.beginTransmission(ADDR_DEVICE);
  Wire.send(CTRL_REG1);
  Wire.send(1 << ACTIVE);
  Wire.endTransmission();*/
  
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

      for(uint8 m = 0;m < MAX_EFFECTS;m++) {
	if(pole.pixel_effects[m] != NULL) {
	  pole.pixels[i] ^= (pole.pixel_effects[m]->update(tick, i, pole.pixels[i], p) << p);
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
