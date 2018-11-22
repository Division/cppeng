//
// Created by Sidorenko Nikita on 1/17/18.
//

#import <EngineMain.h>

std::shared_ptr<Engine> engineInstance = nullptr;

std::shared_ptr<Engine> getEngine() {
  if (!engineInstance) {
    engineInstance = std::make_shared<Engine>();
  }

  return engineInstance;
}
