#include "mutator.h"

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "utils.h"
#include "types.h"

using namespace imgsquash;

const image* mutator::get_primary() const {
  return primary.get();
}

const image* mutator::get_secondary() const {
  return secondary.get();
}

bool mutator::load_image(const std::string &filename) {
  SDL_Surface *surf = IMG_Load(filename.c_str());
  if (surf)
  {
    primary = std::make_unique<image>(surf->w, surf->h);
    primary->fill((u8*)surf->pixels, surf->format->BitsPerPixel == 32);
    
    secondary = std::make_unique<image>(surf->w, surf->h);
    secondary->fill((u32)0);
    
    SDL_FreeSurface(surf);
    return true;
  }
  else return false;
}

void mutator::make_greyscale() {
  r32 *primaryComponent = &primary->data[0];
  r32 *secondaryComponent = &secondary->data[0];
  u32 numPixels = primary->width * primary->height;
  r32 avg;
  for(u32 i=0; i<numPixels; ++i) {
    avg = 0;
    avg += *primaryComponent++;
    avg += *primaryComponent++;
    avg += *primaryComponent++;
    avg += *primaryComponent++;
    avg /= 4.0f;
    
    *secondaryComponent++ = avg;
    *secondaryComponent++ = avg;
    *secondaryComponent++ = avg;
    *secondaryComponent++ = avg;
  }
}

void mutator::make_flat_primary(i32 w, i32 h, u32 color) {
  primary = std::make_unique<image>(w, h);
  secondary = std::make_unique<image>(w, h);
  primary->fill(color);
  secondary->fill((u32)0);
}
