#include "controller.h"
#include "wirish.h"

#ifndef _COLOR_EFFECTS_H_
#define _COLOR_EFFECTS_H_

class ColorEffect {
#define IDLE 0x01
#define RUNNING 0x0
public:
  uint8 status;
  uint16 target_colors[NUM_SIDES][NUM_RGB]; // 3 values for target RGB
  uint16 period; // Time variable
  ColorEffect(uint16 p) {
    period = p;
    status = RUNNING;
  };

  virtual uint16 update(uint16 tick, uint16 side, uint16 channel) =0;
};

class LFade: public ColorEffect {
public:
 LFade(uint16 p): ColorEffect(p) {
    period = p;
    status = RUNNING;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel) {
    return ((target_colors[side][channel] / period) * (tick % period));
  }
};

class CCircle: public ColorEffect {
public:
  int chase_position;
  uint8 chase_length;
  uint8 direction;
 CCircle(uint16 p): ColorEffect(p) {
    period = p;
    chase_length = NUM_SIDES;
    direction = 1;
    if(direction == CHASE_UP) chase_position = 0;
    else chase_position = chase_length-1;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel);
};

#endif
