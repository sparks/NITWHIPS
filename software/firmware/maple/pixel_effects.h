#include "controller.h"
#include "wirish.h"

#ifndef _PIXEL_EFFECTS_H_
#define _PIXEL_EFFECTS_H_

// Abstract base class
class PixelEffect {
#define BLEND_AND 0x00
#define BLEND_OR 0x01
#define BLEND_XOR 0x02
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
