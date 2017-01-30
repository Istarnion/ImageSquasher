#ifndef IMGSQUASH_COMMANDER_H
#define IMGSQUASH_COMMANDER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include <SDL.h>

#include "types.h"
#include "mutator.h"
#include "display.h"

namespace imgsquash {
  class commander {
  private:
    mutator *img_mutator;
    display *gui;

    std::unordered_map<std::string, std::function<void (std::string, std::vector<std::string>)>> command_list;
    std::string user_input;
    
  public:
    commander(mutator *m, display *d);
    
    bool handle_event(SDL_Event *event);
  };
}

#endif // IMGSQUASH_COMMANDER_H
