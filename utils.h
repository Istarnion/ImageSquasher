#ifndef IMAGESQUASH_UTILS_H
#define IMAGESQUASH_UTILS_H

namespace imgsquash {
  template<typename T>
  inline T clamp(T t, T min, T max) {
    if (t < min) return min;
    if (t > max) return max;
    return t;
  }
  
  template<typename T>
  inline T map(T val, T srcMin, T srcMax, T dstMin, T dstMax) {
      return (val - srcMin) * (dstMax - dstMin) / (srcMax - srcMin) + dstMin;
  }
}

#endif // IMAGESQUASH_UTILS_H
