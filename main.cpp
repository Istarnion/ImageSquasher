#include <SDL.h>

#include "display.h"

int main() {
  imgsquash::Display display("Image Squasher", 800, 600);
  
  bool running = true;
  SDL_Event event;
  while(running) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT: running = false; break;
        default: break;
      }
    }
    
    if (running) {
      SDL_Delay(30);
    }
  }
}
