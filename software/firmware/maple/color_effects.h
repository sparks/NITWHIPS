#include "controller.h"
#include "wirish.h"

#ifndef _COLOR_EFFECTS_H_
#define _COLOR_EFFECTS_H_

class ColorEffect {
  public:
    uint16* base_colors;
    uint16 target_colors[NUM_SIDES][NUM_RGB]; // 3 values for target RGB
    uint8 status;
    uint16 period; // Time variable
    uint8 position;
    uint8 offset;
    uint8 length;
    uint8 direction;
    uint8 blend_mode;
    
    ColorEffect();
    uint16 blend(const uint16 o, const uint16 m);
    virtual uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index) =0;
};

/*********************************
 * EFFECTS
 ********************************/

class CStatic: public ColorEffect {
  public:
    CStatic();
    uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index);
};

class CLinearFade: public ColorEffect {
  public:
    CLinearFade();
    uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index);
};

class CCircle: public ColorEffect {
  public:
    CCircle();
    uint16 update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index);
};

#endif
