//
// Created by Sidorenko Nikita on 3/24/18.
//

#ifndef CPPWRAPPER_WINDOW_H
#define CPPWRAPPER_WINDOW_H

#ifndef __EMSCRIPTEN__
#include "SDL.h"
#else
#include <emscripten.h>
#endif

#include "EngMath.h"

class Input;

class Window {
public:
  Window (Input *input): _input(input) {}
  void initOpenGLWindow(int width, int height);
  void swapBuffers();
  void processEvents();
  void quit();

  bool sizeChangedLastFrame() { return _sizeChangedLastFrame; }
  ivec2 size() { return vec2(_width, _height); }
  const vec4 viewport() const { return vec4(0, 0, _width, _height); }

  unsigned int width() const { return (unsigned)_width; }
  unsigned int height() const { return (unsigned)_height; }
  float aspect() const { return (float)_width / (float)_height; }

  bool quitTriggered() { return _quitTriggered; }

private:
#ifndef __EMSCRIPTEN__
  SDL_Window* _window = nullptr;
  SDL_GLContext _context;
  void _processSDLWindowEvent(SDL_WindowEvent &e);
#else
  int _context;
#endif

  Input *_input = nullptr;
  int _width;
  int _height;
  bool _sizeChangedLastFrame = true;

  bool _quitTriggered = false;

  void _updateSize();

};


#endif //CPPWRAPPER_WINDOW_H
