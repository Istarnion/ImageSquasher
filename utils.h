#ifndef IMAGESQUASH_UTILS_H
#define IMAGESQUASH_UTILS_H

namespace imgsquash {
  inline float clamp(float t, float min, float max) {
    if (t < min) return min;
    if (t > max) return max;
    return t;
  }
  
  inline float map(float val, float srcMin, float srcMax, float dstMin, float dstMax) {
      return (val - srcMin) * (dstMax - dstMin) / (srcMax - srcMin) + dstMin;
  }
}

#endif // IMAGESQUASH_UTILS_H
