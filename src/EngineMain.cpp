//
// Created by Sidorenko Nikita on 1/17/18.
//

#import <Engine/EngineMain.h>

Engine *engineInstance = nullptr;

Engine *GetEngine () {
  if (!engineInstance) {
    engineInstance = new Engine();
  }

  return engineInstance;
}

Engine *lerp(float a, float b, float t) {
  return GetEngine();
}

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(EngineMain) {
    emscripten::function("lerp", &lerp, emscripten::allow_raw_pointers());
}
#endif