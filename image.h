#ifndef IMAGESQUASH_IMAGE_H
#define IMAGESQUASH_IMAGE_H

#include <vector>
#include "types.h"

namespace imgsquash {

  struct image {
    std::vector<r32> data;
    const i32 width, height;
    const i32 pitch;
    const i32 channels;
    
    image(i32 w, i32 h): width(w), height(h), pitch(w*sizeof(r32)), channels(4) {
      data.resize(w*h*channels);
    }
    
    void fill(r32 r, r32 g, r32 b, r32 a) {
      for (r32 *pixel=&data[0], *end=&data[data.size()-1]; pixel<end;) {
        *pixel++ = r;
        *pixel++ = g;
        *pixel++ = b;
        *pixel++ = a;
      }
    }
    
    void fill(u32 color) {
      r32 r = (r32)((color & 0xFF000000) >> 24) / 255.0f;
      r32 g = (r32)((color & 0x00FF0000) >> 16) / 255.0f;
      r32 b = (r32)((color & 0x0000FF00) >> 8) / 255.0f;
      r32 a = (r32)(color & 0x000000FF) / 255.0f;
      
      fill(r, g, b, a);
    }
    
    void fill(u8 *pixels, bool hasAlpha) {
      // Trust that we get the right amount of pixels. Scary.
      
      r32 *dst = &data[0];
      i32 numPixels = width * height;
      for(i32 i=0; i<numPixels; ++i) {
        *dst++ = ((*pixels++)) / 255.0f;
        *dst++ = ((*pixels++)) / 255.0f;
        *dst++ = ((*pixels++)) / 255.0f;
        
        if (hasAlpha) {
          *dst++ = ((*pixels++)) / 255.0f;
        }
        else {
          *dst++ = 1;
        }
      }
    }
  };
}
#endif // IMAGESQUASH_IMAGE_H
