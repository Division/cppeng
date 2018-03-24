//
// Created by Sidorenko Nikita on 3/24/18.
//

#include "Input.h"
#include "Logging.h"
#include <unordered_map>

const std::unordered_map<int, int> INPUT_CONVERSION = {
    { (int)SDLK_LEFT, (int)Key::Left },
    { (int)SDLK_RIGHT, (int)Key::Right },
    { (int)SDLK_DOWN, (int)Key::Down},
    { (int)SDLK_UP, (int)Key::Up },
    { (int)SDLK_a, (int)Key::A },
    { (int)SDLK_w, (int)Key::W },
    { (int)SDLK_s, (int)Key::S },
    { (int)SDLK_d, (int)Key::D },
    { (int)SDLK_SPACE, (int)Key::Space },
    { (int)SDLK_ESCAPE, (int)Key::Esc }
};

void Input::updateWithSDLEvent(SDL_Event &e) {
  switch (e.type) {
    case SDL_KEYDOWN:
      _handleSDLKeyState(e.key, true);
      break;

    case SDL_KEYUP:
      _handleSDLKeyState(e.key, false);
      break;

    default:
      // do nothing
      break;
  }
}

void Input::_handleSDLKeyState(SDL_KeyboardEvent &e, bool isDown) {
  if (INPUT_CONVERSION.find(e.keysym.sym) != INPUT_CONVERSION.end()) {
    _keys[INPUT_CONVERSION.at(e.keysym.sym)] = isDown;
  } else if (e.keysym.sym < 256) {
    _keys[e.keysym.sym] = isDown;
  }
  ENGLog("KEY %i", e.keysym.sym);
}
