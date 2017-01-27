#ifndef IMAGESQUASH_COLOR_H
#define IMAGESQUASH_COLOR_H

#include "types.h"

struct color {
  r32 r, g, b, a;
  u32 col;
  
  color(r32 _r, r32 _g, r32 _b, r32 _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {
    col = ((u32)(r * 255.0f) << 24) |
          ((u32)(g * 255.0f) << 16) |
          ((u32)(b * 255.0f) << 8)  |
          ((u32)(a * 255.0f));
  }
  
  color(u32 _col) : col(_col) {
    r = (r32)(col >> 24 & 0xFF) * 255.0f;
    g = (r32)(col >> 16 & 0xFF) * 255.0f;
    b = (r32)(col >> 8  & 0xFF) * 255.0f;
    a = (r32)(col       & 0xFF) * 255.0f;
  }
};

namespace colors {
  const color white(0xFFFFFFFF);
  const color black(0);
  const color red(0xFF0000FF);
  const color green(0x00FF00FF);
  const color blue(0x0000FFFF);
}

#endif // IMAGESQUASH_COLOR_H