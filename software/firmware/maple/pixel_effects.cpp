#include "pixel_effects.h"
#include "controller"
#include "wirish.h"

uint8 Strob::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  if((tick % (period/2)) == 0 && pixel_index == 0) strob ^= 1;
  return strob;
};

uint8 StrobChase::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  if(chase_position == pixel_index) pixel = 0x01;
  else pixel = 0x00;
  if(tick % period == 0 && pixel_index == 0) {
    if(direction == CHASE_UP) {
      chase_position++;
      chase_position %= chase_length;
    } else {
      if(chase_position == 0) chase_position = chase_length;
      chase_position--;
    }
  }
  return pixel;
};
