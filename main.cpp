#include <SDL.h>

#include "display.h"
#include "mutator.h"
#include "commander.h"

int main() {
  imgsquash::display display("Image Squasher", 1280, 768);
  imgsquash::mutator mutator;
  imgsquash::commander commander(&mutator, &display);
  
  if (!display.setup("/home/istarnion/Dev/C++/imagesquasher/res/")) {
    return -1;
  }
  
  bool running = true;
  SDL_Event event;
  while(running) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_WINDOWEVENT:
          display.handle_window_event(event);
          break;
        default:
          commander.handle_event(&event);
          break;
      }
    }
    
    if (running) {
      display.present();
      
      SDL_Delay(30);
    }
  }
}
