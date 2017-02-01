#include "commander.h"

#include <sstream>
#include <iterator>

using namespace imgsquash;

static inline std::string trim(const std::string &s) {
  size_t first = s.find_first_not_of(' ');
  if (std::string::npos == first)
  {
      return s;
  }
  size_t last = s.find_last_not_of(' ');
  return s.substr(first, (last - first + 1));
}

static inline std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                 std::istream_iterator<std::string>());
    return ret;
}

commander::commander(mutator *m, display *d) : img_mutator(m), gui(d) {
  command_list = {
    {"load", [this](std::vector<std::string> &args) -> void{
        if (args.size() > 0) {
          if (!img_mutator->load_image(args[0])) {
            gui->set_error_buffer_string("*ERROR*: Failed to load image " + args[0]);
          }
        }
      }},
    {"invert", [this](std::vector<std::string> &args) -> void{ img_mutator->make_inverted(); } },
    {"grey", [this](std::vector<std::string> &args) -> void{ img_mutator->make_greyscale(); } },
    {"blur", [this](std::vector<std::string> &args) -> void{ img_mutator->make_blur(); } },
    {"sharpen", [this](std::vector<std::string> &args) -> void{ img_mutator->make_sharpen(); } },
    {"outline", [this](std::vector<std::string> &args) -> void{ img_mutator->make_outline(); } },
    {"flip", [this](std::vector<std::string> &args) -> void{ img_mutator->secondary_to_primary(); } },
    {"emboss", [this](std::vector<std::string> &args) -> void{ img_mutator->make_embossed(); } }
  };
}

bool commander::handle_event(SDL_Event *event) {
  bool should_quit = false;
  if (event->type == SDL_KEYUP) {
    auto keycode = event->key.keysym.sym;
    
    if (keycode == SDLK_ESCAPE) {
      should_quit = true;
    }
  }
  else if (event->type == SDL_KEYDOWN) {
    auto keysym = event->key.keysym.sym;
    if (keysym == SDLK_BACKSPACE) {
      if (user_input.length() > 0) {
        user_input.erase(user_input.end()-1);
        gui->set_command_buffer_string(user_input);
      }
    }
    else if (keysym == SDLK_RETURN) {
      parse_command(user_input);
    }
    else if (keysym == SDLK_TAB) {
      // Handle auto complete
    }
  }
  else if (event->type == SDL_TEXTINPUT) {
    user_input.append(event->text.text);
    gui->set_command_buffer_string(user_input);
  }
  
  return should_quit;
}

void commander::parse_command(std::string &command) {
  std::vector<std::string> args = split(trim(command));
  if (args.size() == 0) return;
  
  user_input.erase();
  gui->set_command_buffer_string(user_input);
  
  std::string cmd = args[0];
  if (command_list.find(cmd) != command_list.end()) {
    args.erase(args.begin());
    command_list.at(cmd)(args);
    
    if (img_mutator->get_primary()) gui->set_primary_image(*(img_mutator->get_primary()));
    if (img_mutator->get_secondary()) gui->set_secondary_image(*(img_mutator->get_secondary()));
  }
  else {
    gui->set_error_buffer_string("*ERROR*: Unknown command " + cmd);
  }
}
