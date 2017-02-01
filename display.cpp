#include "display.h"
#include <assert.h>
#include <cstring>
#include <cstdio>
#include <SDL_image.h>
#include "utils.h"
#include "types.h"

using namespace imgsquash;

display::display(const std::string &title, i32 w, i32 h) : width(w), height(h) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(title.c_str(),
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            w, h,
                            SDL_WINDOW_RESIZABLE);
  assert(window);
  
  buffer_width = w/2 - 30;
  buffer_heigth = h - 84;
  
  back_buffer = SDL_GetWindowSurface(window);
  assert(back_buffer);
  
  primary = nullptr;
  secondary = nullptr;
  command_buffer = nullptr;
  error_buffer = nullptr;
  
  bg = SDL_MapRGB(back_buffer->format, 0x2D, 0x2D, 0x2D);
  black = SDL_MapRGB(back_buffer->format, 0, 0, 0);
  
  auto img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
  img_init_flags = IMG_Init(img_init_flags);
  
  if (img_init_flags & IMG_INIT_JPG) puts("JPG:\tOK");
  else puts("JPG:\tFAIL");
  if (img_init_flags & IMG_INIT_PNG) puts("PNG:\tOK");
  else puts("PNG:\tFAIL");
  if (img_init_flags & IMG_INIT_TIF) puts("TIF:\tOK");
  else puts("TIF:\tFAIL");
  if (img_init_flags & IMG_INIT_WEBP) puts("WEBP:\tOK");
  else puts("WEBP:\tFAIL");
  
  TTF_Init();
}

display::~display() {
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  SDL_StopTextInput();
  
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

bool display::setup(const std::string &res_folder_path) {
  font = TTF_OpenFont((res_folder_path + "Tuffy.ttf").c_str(), 18);
  
  SDL_StartTextInput();
  
  return font;
}

void display::blit_rect(i32 x, i32 y, i32 w, i32 h) {
  SDL_Rect dst = {x, y, w, h};
  SDL_FillRect(back_buffer, &dst, black);
}

void display::fit_rect(const SDL_Surface &surf, SDL_Rect &rect) const {
  r32 target_ratio = (r32)buffer_width / (r32)buffer_heigth;
  r32 surf_ratio = (r32)surf.w / (r32)surf.h;
  
  if (target_ratio > surf_ratio) {
    rect.w = (i32)(buffer_heigth * surf_ratio);
    rect.h = buffer_heigth;
  }
  else {
    rect.w = buffer_width;
    rect.h = (i32)(buffer_width / surf_ratio);
  }
}

void display::present() {
  SDL_FillRect(back_buffer, NULL, bg);
  
  SDL_Rect dst = {20, 20, buffer_width, buffer_heigth};
  SDL_FillRect(back_buffer, &dst, black);
  dst.x += buffer_width + 20;
  SDL_FillRect(back_buffer, &dst, black);
  
  if (primary) {
    fit_rect(*primary, dst);
    dst.x = 20 + buffer_width/2 - dst.w/2;
    dst.y = 20 + buffer_heigth/2 - dst.h/2;
    SDL_BlitScaled(primary, NULL, back_buffer, &dst);
    
    if (secondary) {
      dst.x = 40 + buffer_width + buffer_width/2 - dst.w/2;
      SDL_BlitScaled(secondary, NULL, back_buffer, &dst);
    }
  }
  
  if (error_buffer) {
    dst.x = 20;
    dst.y = height - 10 - error_buffer->h;
    SDL_BlitSurface(error_buffer, NULL, back_buffer, &dst);
  }
  else if (command_buffer) {
    dst.x = 20;
    dst.y = height - 10 - command_buffer->h;
    SDL_BlitSurface(command_buffer, NULL, back_buffer, &dst);
  }
  
  SDL_UpdateWindowSurface(window);
}

void display::handle_window_event(const SDL_Event &event) {
  assert(event.type == SDL_WINDOWEVENT);
  
  switch(event.window.event) {
  case SDL_WINDOWEVENT_RESIZED:
      back_buffer = SDL_GetWindowSurface(window);
      width = event.window.data1;
      height = event.window.data2;
      buffer_width = width/2 - 30;
      buffer_heigth = height - 84;
      break;
    default: break;
  }
}

inline i32 display::get_width() const {
  return width;
}

inline i32 display::get_height() const {
  return height;
}

// Forward declare local function
static void convert_image_to_surface(const image &img, SDL_Surface **surf);

void display::set_primary_image(const image &img) {
  if (primary) SDL_FreeSurface(primary);
  convert_image_to_surface(img, &primary);
}

void display::set_secondary_image(const image &img) {
  if (secondary) SDL_FreeSurface(secondary);
  convert_image_to_surface(img, &secondary);
}

void display::set_completion_string(const std::string &completions) {} // TODO: Implement this!

void display::set_command_buffer_string(const std::string &input) {
  error_buffer = nullptr;
  command_buffer = TTF_RenderText_Solid(font, input.c_str(), {255, 255, 255, 255});
}

void display::set_error_buffer_string(const std::string &input) {
  puts("ERROR STRING");
  error_buffer = TTF_RenderText_Solid(font, input.c_str(), {255, 50, 50, 255});
}

static void convert_image_to_surface(const image &img, SDL_Surface **surf) {
  (*surf) = SDL_CreateRGBSurface(0, img.width, img.height, 32,
                                 0xFF000000,
                                 0x00FF0000,
                                 0x0000FF00,
                                 0x000000FF);
  
  const r32 *src = &img.data[0];
  u32 *dst = (u32*)(*surf)->pixels;
  for (int i=0; i<img.width * img.height; ++i) {
    //*dst++ = (u8)(clamp<r32>(*src++, 0, 1) * 255.0f);
    u8 r = (u8)(clamp<r32>(*src++, 0, 1) * 255.0f);
    u8 g = (u8)(clamp<r32>(*src++, 0, 1) * 255.0f);
    u8 b = (u8)(clamp<r32>(*src++, 0, 1) * 255.0f);
    u8 a = (u8)(clamp<r32>(*src++, 0, 1) * 255.0f);
    *dst++ = SDL_MapRGBA((*surf)->format, r, g, b, a);
  }
}
