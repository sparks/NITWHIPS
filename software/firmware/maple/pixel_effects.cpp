#include "pixel_effects.h"
#include "controller.h"
#include "wirish.h"

uint8 PixelEffect::blend(const uint8 o, const uint8 m) {
  switch(blend_mode) {
  case BLEND_AND:
    return o & m;
  case BLEND_OR:
    return o | m;
  case BLEND_XOR:
    return o ^ m;
  default: return 1;
  }
}

uint8 PFullOn::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  return blend(pixel, 0x01); // Always ONNNN.
};

    
uint8 PStrob::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  if((tick % (period/2)) == 0 && pixel_index == 0) position ^= 1;
  return blend(pixel, position);
};

uint8 PChase::update(uint16 tick, uint8 pixel, uint8 pixel_index) {
  uint8 val;
  if(position == pixel_index) val = blend(pixel, 1);
  else val = blend(pixel, 0);
  if(tick % period == 0 && pixel_index == 0) { // Update only once per pixels loop
    if(direction == DIR_UP) position = ++position % length; 
    else {
      if(position == 0) position = length;
      position = --position % length;
    }
  }
  return val;
};
