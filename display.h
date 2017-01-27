#ifndef IMGSQUASH_DISPLAY_H
#define IMGSQUASH_DISPLAY_H

#include <string>
#include <SDL.h>
#include "image.h"

namespace imgsquash {
  class display {
  private:
    SDL_Window *window;
    int width, height;
    int buffer_width, buffer_heigth;
    
    Uint32 bg, black;
    
    SDL_Surface *back_buffer;
    SDL_Surface *primary, *secondary;
    
    void blit_rect(int x, int y, int w, int h);
    void fit_rect(const SDL_Surface &surf, SDL_Rect &rect) const;
    
  public:
    display(const std::string &title, int width, int height);
    ~display();
    
    void present();
    
    void handle_window_event(const SDL_Event &event);
    
    inline int get_width() const;
    inline int get_height() const;
    
    void set_primary_image(const image &img);
    void set_secondary_image(const image &img);
    
    void set_completion_string(const std::string &completions);
    void set_command_buffer_string(const std::string &input);
  };
}

#endif // IMGSQUASH_DISPLAY_H
