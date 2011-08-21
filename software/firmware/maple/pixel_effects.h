#include "controller.h"
#include "wirish.h"

#ifndef _PIXEL_EFFECTS_H_
#define _PIXEL_EFFECTS_H_

// Abstract base class
class PixelEffect {
public:
  uint16 period;
  uint8 position;
  uint8 length;
  uint8 direction;
  uint8 offset;
  PixelEffect(uint16 p) {
    period = p;
  };
  virtual uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index) =0;
};

/*********************************
 * EFFECTS
 ********************************/
class PStrob: public PixelEffect {
public:
 PStrob(uint16 p): PixelEffect(p) {
    period = p;
    position = 1;
  };
  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

class PChase: public PixelEffect {
public:
 PChase(uint16 p): PixelEffect(p) {
    period = p;
    position = NUM_PIXELS;
    direction = DIR_DOWN;
    offset = 0;
    if(direction == DIR_UP) position = offset;
    else position = length-1;
  };

  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

#endif
