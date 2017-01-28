#ifndef IMAGESQUASH_MUTATOR_H
#define IMAGESQUASH_MUTATOR_H

#include <memory>
#include <string>

#include "image.h"
#include "types.h"
#include "color.h"

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
    void make_inverted();
    void make_identity();
    void make_sharpen();
    void make_blur();
    void make_flat_primary(i32 w, i32 h, color col);
  };
}

#endif // IMAGESQUASH_MUTATOR_H
