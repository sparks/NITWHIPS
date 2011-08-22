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
    PixelEffect();
    uint8 blend(const uint8 o, const uint8 m);
    virtual uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index) =0;
};

/*********************************
 * EFFECTS
 ********************************/
 
class PFullOn: public PixelEffect {
  public:
    PFullOn();
    uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

class PStrob: public PixelEffect {
  public:
    PStrob();
    uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

class PChase: public PixelEffect {
  public:
    PChase();
    uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index);
};

#endif
