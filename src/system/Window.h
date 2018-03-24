//
// Created by Sidorenko Nikita on 3/24/18.
//

#ifndef CPPWRAPPER_WINDOW_H
#define CPPWRAPPER_WINDOW_H

#include "SDL.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Input;

class Window {
public:
  Window (Input *input): _input(input) {}
  void initOpenGLWindow();
  void swapBuffers();
  void processEvents();
  void quit();

  bool quitTriggered() { return _quitTriggered; }

private:
  SDL_Window* _window;
  SDL_GLContext _context;
  Input *_input;

  bool _quitTriggered;
};


#endif //CPPWRAPPER_WINDOW_H
