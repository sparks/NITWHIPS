#include "controller.h"
#include "wirish.h"

#ifndef _COLOR_EFFECTS_H_
#define _COLOR_EFFECTS_H_

class ColorEffect {
 public:
  uint16 * base_colors;
  uint16 target_colors[NUM_SIDES][NUM_RGB]; // 3 values for target RGB
  uint8 status;
  uint16 period; // Time variable
  uint8 position;
  uint8 offset;
  uint8 length;
  uint8 direction;
  uint8 blend_mode;
  ColorEffect(uint16 p) {
    period = p;
    status = RUNNING;
  };
  uint16 blend(const uint16 o, const uint16 m);
  virtual uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index) =0;
};

class CStatic: public ColorEffect {
 public:
 CStatic(uint16 p): ColorEffect(p) {
    period = p;
    blend_mode = BLEND_OR;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index);
};

class CLinearFade: public ColorEffect {
 public:
 CLinearFade(uint16 p): ColorEffect(p) {
    period = p;
    status = RUNNING;
    blend_mode = BLEND_AND;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index);
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
  uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index);
};

#endif
