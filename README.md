# Image Squasher

This is a simple tool I'm making to learn more about image processing.

It is very much a work in progress, but the goal is to have a useful utility for enhancing images through filters, while getting quick feedback in the GUI.

## Architecture
The program has three distinct modules;
- Display - Handles the window and draws everything
- Controller - Handles user input and sends command to the other modules
- Mutator - Where the actual magic happens. All filter effects are implemented here, and this module is entirely independent of the other two, though it still depends on SDL and SDL_image.

## Dependencies
- SDL2
- SDL_image

On any POSIX OS, these should be available in your package manager.

On Windows, you will need the appropriate *.dll.

## Compiling
```
mkdir build
cd build
cmake ..
make
```

This is probably different on Windows. I'm sorry.
