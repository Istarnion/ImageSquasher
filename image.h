#ifndef IMAGESQUASH_IMAGE_H
#define IMAGESQUASH_IMAGE_H

#include <vector>

namespace imgsquash {

  struct image {
    std::vector<float> data;
    const int width, height;
    const int pitch;
    const int channels;
    
    image(int w, int h): width(w), height(h), pitch(w*sizeof(float)), channels(4) {
      data.resize(w*h*channels);
    }
    
    void fill(float r, float g, float b, float a) {
      for (float *pixel=&data[0], *end=&data[data.size()-1]; pixel<end;) {
        *pixel++ = r;
        *pixel++ = g;
        *pixel++ = b;
        *pixel++ = a;
      }
    }
    
    void fill(unsigned int color) {
      float r = (float)(color & 0xFF000000) / 255.0f;
      float g = (float)(color & 0x00FF0000) / 255.0f;
      float b = (float)(color & 0x0000FF00) / 255.0f;
      float a = (float)(color & 0x000000FF) / 255.0f;
      
      fill(r, g, b, a);
    }
    
    void fill(unsigned int *pixels) {
      // Trust that we get the right amount of pixels. Scary.
      
      float *dst = &data[0];
      int numPixels = width * height;
      for(int i=0; i<numPixels; ++i) {
        *dst++ = ((*pixels) & 0xFF000000) / 255.0f;
        *dst++ = ((*pixels) & 0x00FF0000) / 255.0f;
        *dst++ = ((*pixels) & 0x0000FF00) / 255.0f;
        *dst++ = ((*pixels) & 0x000000FF) / 255.0f;
        ++pixels;
      }
    }
  };
}
#endif // IMAGESQUASH_IMAGE_H
