#include "pixel_effects.h"
#include "controller.h"
#include "wirish.h"

uint8 PStrob::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  if((tick % (period/2)) == 0 && pixel_index == 0) position ^= 1;
  return position;
};

uint8 PChase::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  if(position == pixel_index) pixel = 0x01;
  else pixel = 0x00;
  if(tick % period == 0 && pixel_index == 0) { // Update only once per pixels loop
    if(direction == DIR_UP) {
      position++;
      position = (position % length) + offset;
    } else {
      if(position == 0) position = length-1;
      position--;
    }
  }
  return pixel;
};
