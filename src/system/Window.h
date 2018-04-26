//
// Created by Sidorenko Nikita on 3/24/18.
//

#ifndef CPPWRAPPER_WINDOW_H
#define CPPWRAPPER_WINDOW_H

#include "SDL.h"
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
  ivec2 size() {}
  const vec4 viewport() const { return vec4(0, 0, _width, _height); }

  unsigned int width() { return (unsigned)_width; }
  unsigned int height() { return (unsigned)_height; }
  float aspect() { return (float)_width / (float)_height; }

  bool quitTriggered() { return _quitTriggered; }

private:
  SDL_Window* _window = nullptr;
  Input *_input = nullptr;
  SDL_GLContext _context;

  int _width;
  int _height;
  bool _sizeChangedLastFrame = true;

  bool _quitTriggered = false;

  void _updateSize();

  void _processSDLWindowEvent(SDL_WindowEvent &e);
};


#endif //CPPWRAPPER_WINDOW_H
