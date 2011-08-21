#include "color_effects.h"
#include "controller.h"
#include "wirish.h"

uint16 CCircle::update(uint16 tick, uint16 side, uint16 channel) {
    uint16 mask;
    if(chase_position == side) mask = 0xFFFF;
    else mask = 0x0000;
    if(tick % period == 0 && side == 0 && channel == 0) {
      if(direction == 1) {
	chase_position++;
	chase_position %= chase_length;
      } else {
	if(chase_position == 0) chase_position = chase_length;
	chase_position--;
      }
    }
    return mask;
  }
