#include "display.h"

using namespace imgsquash;

Display::Display(const std::string &title, int w, int h) : width(w), height(h) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
}

Display::~Display() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Display::handle_window_event(const SDL_Event &event) {
}

inline int Display::get_width() const {
  return width;
}

inline int Display::get_height() const {
  return height;
}

void Display::set_primary_image(const SDL_Surface &surface) {}
void Display::set_secondary_image(const SDL_Surface &surface) {}

void Display::set_completion_string(const std::string &completions) {}
void Display::set_command_buffer_string(const std::string &input) {}
