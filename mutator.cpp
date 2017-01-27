#include "mutator.h"

#include <memory>
#include <string>
#include <cstdio>
#include <algorithm>

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
    
    // This is quite inefficient, but very much easier on the me, so idk.
    SDL_Surface *temp_surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surf);
    
    u8 r, g, b, a;
    r32 *dst_pixel = &primary->data[0];
    i32 num_pixels = surf->w * surf->h;
    u32 *src_pixels = ((u32*)temp_surf->pixels);
    SDL_PixelFormat *format = temp_surf->format;
    
    for (i32 i=0; i<num_pixels; ++i) {
      SDL_GetRGBA(*src_pixels++, format, &r, &g, &b, &a);
      *dst_pixel++ = r / 255.0f;
      *dst_pixel++ = g / 255.0f;
      *dst_pixel++ = b / 255.0f;
      *dst_pixel++ = a / 255.0f;
    }
     
    SDL_FreeSurface(temp_surf);
    
    secondary = std::make_unique<image>(surf->w, surf->h);
    secondary->clear();
    
    return true;
  }
  else return false;
}

void mutator::make_greyscale() {
  r32 *primaryComponent = &primary->data[0];
  r32 *secondaryComponent = &secondary->data[0];
  u32 numPixels = primary->width * primary->height;
  r32 avg;
  r32 alpha;
  for(u32 i=0; i<numPixels; ++i) {
    avg = 0;
    avg += (*primaryComponent++) * 0.2126f;
    avg += (*primaryComponent++) * 0.7152f;
    avg += (*primaryComponent++) * 0.0722f;
    alpha = (*primaryComponent++);
    
    *secondaryComponent++ = avg;
    *secondaryComponent++ = avg;
    *secondaryComponent++ = avg;
    *secondaryComponent++ = alpha;
  }
}

void mutator::make_identity() {
  std::copy(primary->data.begin(), primary->data.end(), secondary->data.begin());
}
 
void mutator::make_flat_primary(i32 w, i32 h, color col) {
  primary = std::make_unique<image>(w, h);
  secondary = std::make_unique<image>(w, h);
  primary->fill(col);
  secondary->fill((u32)0);
  
  primary->blitRect(0, 0, 400, 300, color(0, 0.5f, 0.5f));
}
