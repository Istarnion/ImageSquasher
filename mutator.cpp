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

static void convolve_kernel3(const image &src, image &dst, const r32 *kernel) {
  const i32 num_channels = src.channels;
  const i32 width = src.width;
  const i32 height = src.height;
  const i32 num_pixels = width * height;
  const r32 *src_pixel = &src.data[0];
  r32 *dst_pixel = &dst.data[0];
  
  r32 dst_r, dst_g, dst_b, dst_a;
  i32 start_index;
  
  i32 x, y;
  
  for (i32 i=0; i<num_pixels; ++i) {
    x = i % width;
    y = i / width;
    dst_r = dst_g = dst_b = dst_a = 0;

    // top left
    if (x > 0 && y > 0) {
      start_index = (x-1) + (y-1) * width;
      start_index *= num_channels;
      dst_r = src_pixel[start_index]   * kernel[0];
      dst_g = src_pixel[start_index+1] * kernel[0];
      dst_b = src_pixel[start_index+2] * kernel[0];
    }
    
    // top
    if (y > 0) {
      start_index = x + (y-1) * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[1];
      dst_g += src_pixel[start_index+1] * kernel[1];
      dst_b += src_pixel[start_index+2] * kernel[1];
    }
    
    // top right
    if (x < width-1 && y > 1) {
      start_index = (x+1) + (y-1) * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[2];
      dst_g += src_pixel[start_index+1] * kernel[2];
      dst_b += src_pixel[start_index+2] * kernel[2];
    }
    
    // left
    if (x > 0) {
      start_index = (x-1) + y * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[3];
      dst_g += src_pixel[start_index+1] * kernel[3];
      dst_b += src_pixel[start_index+2] * kernel[3];
    }
    
    // center
    start_index = num_channels * (x + y * width);
    dst_r += src_pixel[start_index]   * kernel[4];
    dst_g += src_pixel[start_index+1] * kernel[4];
    dst_b += src_pixel[start_index+2] * kernel[4];
    dst_a += src_pixel[start_index+3];
    
    // right
    if (x < width-1) {
      start_index = (x+1) + y * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[5];
      dst_g += src_pixel[start_index+1] * kernel[5];
      dst_b += src_pixel[start_index+2] * kernel[5];
    }
    
    // lower left
    if (x > 0 && y < height-1) {
      start_index = (x-1) + (y+1) * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[6];
      dst_g += src_pixel[start_index+1] * kernel[6];
      dst_b += src_pixel[start_index+2] * kernel[6];
    }
    
    // lower
    if (y < height - 1) {
      start_index = x + (y+1) * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[7];
      dst_g += src_pixel[start_index+1] * kernel[7];
      dst_b += src_pixel[start_index+2] * kernel[7];
    }
    
    // lower right
    if (x < width - 1 && y < height - 1) {
      start_index = (x+1) + (y+1) * width;
      start_index *= num_channels;
      dst_r += src_pixel[start_index]   * kernel[8];
      dst_g += src_pixel[start_index+1] * kernel[8];
      dst_b += src_pixel[start_index+2] * kernel[8];
    }
    
    // Set values to destination
    *dst_pixel++ = dst_r;
    *dst_pixel++ = dst_g;
    *dst_pixel++ = dst_b;
    *dst_pixel++ = dst_a;
  }
}

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
  if (!primary) return;
  
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

void mutator::make_inverted() {
  if (!primary) return;
  
  r32 *primaryComponent = &primary->data[0];
  r32 *secondaryComponent = &secondary->data[0];
  u32 numPixels = primary->width * primary->height;
  r32 r, g, b;
  r32 alpha;
  for(u32 i=0; i<numPixels; ++i) {
    r = 1.0f - (*primaryComponent++);
    g = 1.0f - (*primaryComponent++);
    b = 1.0f - (*primaryComponent++);
    alpha = (*primaryComponent++);
    
    *secondaryComponent++ = r;
    *secondaryComponent++ = g;
    *secondaryComponent++ = b;
    *secondaryComponent++ = alpha;
  }
}

void mutator::make_identity() {
  if (!primary) return;
  
  std::copy(primary->data.begin(), primary->data.end(), secondary->data.begin());
}

void mutator::make_sharpen() {
  if (!primary) return;
  
  r32 kernel[9] = {
     0.0f, -1.0f,  0.0f,
    -1.0f,  5.0f, -1.0f,
     0.0f, -1.0f,  0.0f
  };
  
  convolve_kernel3(*primary, *secondary, kernel);
}

void mutator::make_blur() {
  if (!primary) return;
  
  r32 kernel[9] = {
    0.0625f, 0.1250f, 0.0625f,
    0.1250f, 0.2500f, 0.1250f,
    0.0625f, 0.1250f, 0.0625f
  };
  
  convolve_kernel3(*primary, *secondary, kernel);
}

void mutator::make_embossed() {
  if (!primary) return;
  
  r32 kernel[9] = {
    -2, -1,  0,
    -1,  1,  1,
     0,  1,  2
  };
  
  convolve_kernel3(*primary, *secondary,kernel);
}

void mutator::make_outline() {
  if (!primary) return;
  
  r32 kernel[9] = {
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
  };
  
  convolve_kernel3(*primary, *secondary,kernel);
}

void mutator::secondary_to_primary() {
  if (!primary || !secondary) return;
  
  primary = std::make_unique<image>(*secondary);
  secondary = std::make_unique<image>(primary->width, primary->height);
}

void mutator::make_flat_primary(i32 w, i32 h, color col) {
  primary = std::make_unique<image>(w, h);
  secondary = std::make_unique<image>(w, h);
  primary->fill(col);
  secondary->fill((u32)0);
  
  primary->blitRect(0, 0, 400, 300, color(0, 0.5f, 0.5f));
}
