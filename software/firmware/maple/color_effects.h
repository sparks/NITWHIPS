#include "controller.h"
#include "wirish.h"

#ifndef _COLOR_EFFECTS_H_
#define _COLOR_EFFECTS_H_

class ColorEffect {
 public:
  uint16 target_colors[NUM_SIDES][NUM_RGB]; // 3 values for target RGB
  uint8 status;
  uint16 period; // Time variable
  uint8 position;
  uint8 offset;
  uint8 length;
  uint8 direction;
  ColorEffect(uint16 p) {
    period = p;
    status = RUNNING;
  };
  
  virtual uint16 update(uint16 tick, uint16 side, uint16 channel) =0;
};

class CLinearFade: public ColorEffect {
 public:
 CLinearFade(uint16 p): ColorEffect(p) {
    period = p;
    status = RUNNING;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel);
};

class CCircle: public ColorEffect {
 public:
 CCircle(uint16 p): ColorEffect(p) {
    period = p;
    length = NUM_SIDES;
    direction = DIR_UP;
    offset = 0;
    if(direction == DIR_UP) position = offset;
    else position = length-1;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel);
};

#endif
