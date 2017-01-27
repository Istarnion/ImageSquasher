#ifndef IMAGESQUASH_MUTATOR_H
#define IMAGESQUASH_MUTATOR_H

#include <memory>
#include <string>

#include "image.h"

namespace imgsquash {
  class mutator {
  private:
    std::unique_ptr<image> primary;
    std::unique_ptr<image> secondary;
    
  public:
    mutator() {}
    
    const image *get_primary() const;
    const image *get_secondary() const;
    
    bool load_image(const std::string &filename);
    void make_greyscale();
    void make_flat_primary(int w, int h, unsigned int color);
  };
}

#endif // IMAGESQUASH_MUTATOR_H