#include "color_effects.h"
#include "controller.h"
#include "wirish.h"

uint16 ColorEffect::blend(const uint16 o, const uint16 m) {
  switch(blend_mode) {
  case BLEND_AND:
    return o & m;
  case BLEND_OR:
    return o | m;
  case BLEND_XOR:
    return o ^ m;
  default: return 0xFFFF;
  }
}

uint16 CStatic::update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index) {
  return blend(channel, target_colors[side][channel_index]);
}

uint16 CLinearFade::update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index) {
  uint16 val;
  val = (target_colors[side][channel_index] / period) * (tick % period);
  return blend(channel, val);
}

uint16 CCircle::update(uint16 tick, uint16 side, uint16 channel, uint8 channel_index) {
    uint16 val;
    if(position == side) val = blend(channel, 0xFFFF);
    else val = blend(channel, 0x0000);
    if(tick % period == 0 && side == 0 && channel_index == 0) {
      if(direction == DIR_UP) position = ++position % length;
      else {
	if(position == 0) position = length;
	position = --position % length;
      }
    }
    return val;
  }
