#include "commander.h"

using namespace imgsquash;

commander::commander(mutator *m, display *d) : img_mutator(m), gui(d) {
  // Setup commands
}

bool commander::handle_event(SDL_Event *event) {
  return false;
}
