#include "controller.h"
#include "wirish.h"

#ifndef _PIXEL_EFFECTS_H_
#define _PIXEL_EFFECTS_H_

// Abstract base class
class PixelEffect {
public:
  uint16 period;
  PixelEffect(uint16 p) {
    period = p;
  };
  virtual uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) =0;
};

/*********************************
 * EFFECTS
 ********************************/
class Strob: public PixelEffect {
public:
  Strob(uint16 p): PixelEffect(p) {
    period = p;
  };
  uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) {
    pixel &= (1 << pixel_index);
    pixel >>= pixel_index;
    
    if((tick % (period/2)) == 0) pixel ^= 1;
    
    return pixel;

  };
};

class StrobChase: public PixelEffect {
public:
  uint8 chase_position;
  uint8 chase_length;
  uint8 pixel_state;
  StrobChase(uint16 p): PixelEffect(p) {
    period = p;
    chase_position = 0x01;
    chase_length = 1 << (NUM_PIXELS-1);
  };
  uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) {
    if(pixel_index == 0) {
      chase_position <<= 1;
      if(chase_position > chase_length) chase_position = 0x01;
    }
    pixel &= (1 << pixel_index);
    pixel &= chase_position;
    pixel >>= pixel_index;
    
    return pixel;
  };
};

#endif
