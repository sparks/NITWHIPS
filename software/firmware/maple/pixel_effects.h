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
  uint8 blend_mode;
  PixelEffect(uint16 p) {
    period = p;
  };
  uint8 blend(const uint8 o, const uint8 m);
  virtual uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index) =0;
};

/*********************************
 * EFFECTS
 ********************************/
class PFullOn: public PixelEffect {
public:
 PFullOn(uint16 p): PixelEffect(p) {
    period = p;
    blend_mode = BLEND_OR;
  };
  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

class PStrob: public PixelEffect {
public:
 PStrob(uint16 p): PixelEffect(p) {
    period = p;
    position = 1;
    blend_mode = BLEND_AND;
  };
  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

class PChase: public PixelEffect {
public:
 PChase(uint16 p): PixelEffect(p) {
    period = p;
    offset = 0;
    position = offset;
    length = 0;
    direction = DIR_UP;
    blend_mode = BLEND_AND;
  };

  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

#endif
