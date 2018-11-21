//
// Created by Sidorenko Nikita on 3/24/18.
//

#ifndef CPPWRAPPER_WINDOW_H
#define CPPWRAPPER_WINDOW_H

#include "EngineGL.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "EngMath.h"

class Input;

class Window {
public:
  Window() = default;
  void initOpenGLWindow(int width, int height);
  void swapBuffers();
  void processEvents();
  void quit();

  ivec2 size() { return vec2(_width, _height); }
  const vec4 viewport() const { return vec4(0, 0, _width, _height); }

  unsigned int width() const { return (unsigned)_width; }
  unsigned int height() const { return (unsigned)_height; }
  float aspect() const { return (float)_width / (float)_height; }
  GLFWwindow *glfwID() const { return _window; }

  bool quitTriggered() { return (bool)glfwWindowShouldClose(_window); }

private:
  GLFWwindow* _window = nullptr;
  int _width = 0;
  int _height = 0;
  void _updateSize();
};


#endif //CPPWRAPPER_WINDOW_H
