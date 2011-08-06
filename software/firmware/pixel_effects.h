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
  uint8 state;
  Strob(uint16 p): PixelEffect(p) {
    period = p;
    state = 1;
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
  StrobChase(uint16 p): PixelEffect(p) {
    period = p;
    chase_position = 0;
    chase_length = NUM_PIXELS - 1;
  };
  uint8 update(uint16 tick, uint16 side, uint8 pixel, uint8 pixel_index) {

    if((pixel_index == 0) && (tick % period == 0)) {
      
    }
    pixel &= (1 << pixel_index);
    pixel &= (1 << chase_position);
    pixel >>= pixel_index;
    
    return pixel;
  };
};

#endif
