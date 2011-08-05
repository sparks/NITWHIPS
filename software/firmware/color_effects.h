#include "controller.h"
#include "wirish.h"

#ifndef _COLOR_EFFECTS_H_
#define _COLOR_EFFECTS_H_

class ColorEffect {
public:
  uint16 target_color[NUM_RGB];
  uint16 period;
  ColorEffect(uint16 p) {
    period = p;
  };
  virtual uint16 update(uint16 tick, uint16 side, uint16 channel) =0;
};

class LFade: public ColorEffect {
public:
 LFade(uint16 p): ColorEffect(p) {
    period = p;
  };
  uint16 update(uint16 tick, uint16 side, uint16 channel) {
    return (0xFFFF / period) * (tick % period);
  }
};

#endif
