#ifndef IMGSQUASH_DISPLAY_H
#define IMGSQUASH_DISPLAY_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "image.h"
#include "types.h"

namespace imgsquash {
  class display {
  private:
    SDL_Window *window;
    i32 width, height;
    i32 buffer_width, buffer_heigth;
    
    Uint32 bg, black;
    
    SDL_Surface *back_buffer;
    SDL_Surface *primary, *secondary;
    SDL_Surface *command_buffer;
    SDL_Surface *error_buffer;
    
    TTF_Font *font;
    
    void blit_rect(i32 x, i32 y, i32 w, i32 h);
    void fit_rect(const SDL_Surface &surf, SDL_Rect &rect) const;
    
  public:
    display(const std::string &title, i32 width, i32 height);
    ~display();
    
    bool setup(const std::string &res_folder_path);
    
    void present();
    
    void handle_window_event(const SDL_Event &event);
    
    inline i32 get_width() const;
    inline i32 get_height() const;
    
    void set_primary_image(const image &img);
    void set_secondary_image(const image &img);
    
    void set_completion_string(const std::string &completions);
    void set_command_buffer_string(const std::string &input);
    void set_error_buffer_string(const std::string &input);
  };
}

#endif // IMGSQUASH_DISPLAY_H
