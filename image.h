#ifndef IMAGESQUASH_IMAGE_H
#define IMAGESQUASH_IMAGE_H

#include <vector>
#include <cstring>
#include "types.h"
#include "utils.h"
#include "color.h"

namespace imgsquash {

  struct image {
    std::vector<r32> data;
    const i32 width, height;
    const i32 pitch;
    const i32 channels;
    
    image(i32 w, i32 h): width(w), height(h), pitch(w*sizeof(r32)), channels(4) {
      data.resize(w*h*channels);
    }

    void clear() {
      memset(&data[0], 0, data.size());
    }
    
    void fill(color col) {
      for (r32 *pixel=&data[0], *end=&data[data.size()-1]; pixel<end;) {
        *pixel++ = col.r;
        *pixel++ = col.g;
        *pixel++ = col.b;
        *pixel++ = col.a;
      }
    }
    
    void fill(u8 *pixels, u8 bytesPerPixel) {
      // Trust that we get the right amount of pixels. Scary.
      
      r32 *dst = &data[0];
      i32 numPixels = width * height;
      for(i32 i=0; i<numPixels; ++i) {
        for(i32 b=0; b<bytesPerPixel; ++b) {
          *dst++ = (*pixels++) / 255.0f;
        }
        for(i32 b=0; b<4-bytesPerPixel; ++b) {
          *dst++ = 1.0f;
        }
      }
    }
    
    void blitRect(i32 x, i32 y, i32 w, i32 h, color col) {
      x = clamp<i32>(x, 0, width) * channels;
      w = clamp<i32>(w, 0, width) * channels;
      y = clamp<i32>(y, 0, height);
      h = clamp<i32>(h, 0, height);
      
      for(i32 row=y; row<y+h; ++row) {
        for(i32 c=x; c<x+w; c += channels) {
          float *pixel = &data[row*width*channels + c];
          *pixel++ = col.r;
          *pixel++ = col.g;
          *pixel++ = col.b;
          *pixel++ = col.a;
        }
      }
    }
  };
}
#endif // IMAGESQUASH_IMAGE_H
