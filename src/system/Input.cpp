//
// Created by Sidorenko Nikita on 3/24/18.
//

#include "Input.h"
#include "Logging.h"
#include <unordered_map>
#include "EngMath.h"

const std::unordered_map<int, int> INPUT_CONVERSION = {
    { (int)Key::Left, GLFW_KEY_LEFT },
    { (int)Key::Right, GLFW_KEY_RIGHT },
    { (int)Key::Down, GLFW_KEY_DOWN},
    { (int)Key::Up, GLFW_KEY_UP },
    { (int)Key::A, GLFW_KEY_A },
    { (int)Key::W, GLFW_KEY_W },
    { (int)Key::S, GLFW_KEY_S },
    { (int)Key::D, GLFW_KEY_D },
    { (int)Key::E, GLFW_KEY_E },
    { (int)Key::Q, GLFW_KEY_Q },
    { (int)Key::C, GLFW_KEY_C },
    { (int)Key::X, GLFW_KEY_X },
    { (int)Key::Z, GLFW_KEY_Z },
    { (int)Key::Space, GLFW_KEY_SPACE },
    { (int)Key::Esc, GLFW_KEY_ESCAPE },
    { (int)Key::Equal, GLFW_KEY_EQUAL},

    { (int)Key::MouseLeft, GLFW_MOUSE_BUTTON_1 },
    { (int)Key::MouseRight, GLFW_MOUSE_BUTTON_2 }
};

void Input::_update() {
  double x, y;
  glfwGetCursorPos(_window->glfwID(), &x, &y);
  _prevMousePos = _mousePos;
  _mousePos = vec2(x, y);
  _mouseDelta = _mousePos - _prevMousePos;
}

int Input::keyDown(Key key) const {
  switch (key) {
    case Key::MouseLeft:
    case Key::MouseRight:
      return mouseDown(INPUT_CONVERSION.at((int)key));

    default:
      return keyDown(INPUT_CONVERSION.at((int)key));
  }

}