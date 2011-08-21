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
  virtual uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index) =0;
};

/*********************************
 * EFFECTS
 ********************************/
class Strob: public PixelEffect {
public:
  Strob(uint16 p): PixelEffect(p) {
    period = p;
  };
  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index) {
    if((tick % (period/2)) == 0) pixel ^= 1;
    return pixel;
  };
};

class StrobChase: public PixelEffect {
#define CHASE_DOWN -1
#define CHASE_UP 1
public:
  int chase_position;
  uint8 chase_length;
  uint8 direction;
  StrobChase(uint16 p): PixelEffect(p) {
    period = p;
    chase_position = 0;
    chase_length = NUM_PIXELS;
    direction = CHASE_DOWN;
  };

  uint8 update(uint16 tick, uint8 pixel, uint8 pixel_index) {
    if(chase_position == pixel_index) pixel = 0x01;
    else pixel = 0x00;
    if(tick % period == 0 && pixel_index == 0) {
      chase_position += 1 * direction;
      chase_position %= chase_length;
    }
    return pixel;
  };
};

#endif
