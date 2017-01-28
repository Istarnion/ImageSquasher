#include <SDL.h>

#include "display.h"
#include "mutator.h"
#include "color.h"

int main() {
  imgsquash::display display("Image Squasher", 1280, 768);
  
  // Do some stuff for testing until we implement the command module
  imgsquash::mutator mutator;
  bool result = mutator.load_image("/home/istarnion/Dev/C++/imagesquasher/res/einstein.jpg");

  if (result) {
    mutator.make_embossed();
    
    auto primaryImg = mutator.get_primary();
    display.set_primary_image(*primaryImg);
    
    auto secondaryImg = mutator.get_secondary();
    display.set_secondary_image(*secondaryImg);
  }
  
  bool running = true;
  SDL_Event event;
  while(running) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT: running = false; break;
        case SDL_WINDOWEVENT: display.handle_window_event(event);
        default: break;
      }
    }
    
    if (running) {
      display.present();
      
      SDL_Delay(30);
    }
  }
}
