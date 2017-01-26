#ifndef IMGSQUASH_DISPLAY_H
#define IMGSQUASH_DISPLAY_H

#include <string>

#include <SDL.h>

namespace imgsquash {
  class Display {
  private:
    SDL_Window *window;
    int width, height;
    
  public:
    Display(const std::string &title, int width, int height);
    ~Display();
    
    void handle_window_event(const SDL_Event &event);
    
    inline int get_width() const;
    inline int get_height() const;
    
    void set_primary_image(const SDL_Surface &surface);
    void set_secondary_image(const SDL_Surface &surface);
    
    void set_completion_string(const std::string &completions);
    void set_command_buffer_string(const std::string &input);
  };
}

#endif // IMGSQUASH_DISPLAY_H
