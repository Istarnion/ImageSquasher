#include "mutator.h"

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "utils.h"

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
    primary->fill((Uint32*)surf->pixels);
    
    secondary = std::make_unique<image>(surf->w, surf->h);
    secondary->fill((Uint32)0);
    
    SDL_FreeSurface(surf);
    return true;
  }
  else return false;
}

void mutator::make_greyscale() {
  float *primaryComponent = &primary->data[0];
  float *secondaryComponent = &secondary->data[0];
  size_t numPixels = primary->width * primary->height;
  float avg;
  for(size_t i=0; i<numPixels; ++i) {
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

void mutator::make_flat_primary(int w, int h, Uint32 color) {
  primary = std::make_unique<image>(w, h);
  secondary = std::make_unique<image>(w, h);
  primary->fill(color);
  secondary->fill((Uint32)0);
}
