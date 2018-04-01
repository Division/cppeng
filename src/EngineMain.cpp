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
