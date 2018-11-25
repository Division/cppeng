//
// Created by Sidorenko Nikita on 3/24/18.
//

#ifndef CPPWRAPPER_INPUT_H
#define CPPWRAPPER_INPUT_H

#include "glm/glm.hpp"
#include "EngineGL.h"
#include "Window.h"
#include <memory>

using namespace glm;

enum class Key : int {
  Down = 0,
  Up,
  Left,
  Right,
  A,
  D,
  W,
  S,
  E,
  Q,
  C,
  X,
  Z,
  Space,
  Esc,
  Equal,

  MouseLeft,
  MouseRight,
};

class Window;

class Input {
public:
  friend class Engine;

  explicit Input(const std::shared_ptr<Window> window): _window(window) {}
  int keyDown(Key key) const;
  int keyDown(int key) const { return glfwGetKey(_window->glfwID(), key); }
  int mouseDown(int key) const { return glfwGetMouseButton(_window->glfwID(), key); }
  const vec2 mousePosition() const { return _mousePos; }
  const vec2 mouseDelta() const { return _mouseDelta; }
private:
  void _update();
  std::shared_ptr<Window>_window;
  vec2 _mousePos;
  vec2 _prevMousePos;
  vec2 _mouseDelta;

private:

};


#endif //CPPWRAPPER_INPUT_H
