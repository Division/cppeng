//
// Created by Sidorenko Nikita on 3/24/18.
//

#ifndef CPPWRAPPER_INPUT_H
#define CPPWRAPPER_INPUT_H

#import "SDL.h"
#import "glm/glm.hpp"

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
  Space,
  Esc,

  MouseLeft,
  MouseRight,
};

class Input {
public:
  friend class Window;
  bool keyDown(Key key) const { return _keys[(int)key]; }
private:
  bool _keys[256];

private:
  void updateWithSDLEvent(SDL_Event &e);
  void _handleSDLKeyState(SDL_KeyboardEvent &e, bool isDown);

  void _handleSDLMouseState(SDL_MouseButtonEvent &e, bool isDown);
};


#endif //CPPWRAPPER_INPUT_H
