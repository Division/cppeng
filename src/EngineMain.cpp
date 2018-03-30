//
// Created by Sidorenko Nikita on 1/17/18.
//

#import <Engine/EngineMain.h>

Engine *engineInstance = nullptr;

Engine *getEngine() {
  if (!engineInstance) {
    engineInstance = new Engine();
  }

  return engineInstance;
}

Engine *lerp(float a, float b, float t) {
  return getEngine();
}

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(EngineMain) {
  emscripten::function("lerp", &lerp, emscripten::allow_raw_pointers());
}
#endif