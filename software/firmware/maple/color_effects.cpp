#include "color_effects.h"
#include "controller.h"
#include "wirish.h"

uint16 CLinearFade::update(uint16 tick, uint16 side, uint16 channel) {
  return ((target_colors[side][channel] / period) * (tick % period));
}

uint16 CCircle::update(uint16 tick, uint16 side, uint16 channel) {
    uint16 mask;
    if(position == side) mask = 0xFFFF;
    else mask = 0x0000;
    if(tick % period == 0 && side == 0 && channel == 0) {
      if(direction == DIR_UP) {
	position++;
	position %= length;
      } else {
	if(position == DIR_DOWN) position = length;
	position--;
      }
    }
    return mask;
  }
