//
// Created by Sidorenko Nikita on 11/21/18.
//

#ifndef CPPWRAPPER_ICAMERAPARAMSPROVIDER_H
#define CPPWRAPPER_ICAMERAPARAMSPROVIDER_H

#include "EngMath.h"

// Scenes are rendered not only from the Camera scene objects, but also from the light's point of view
// This abstraction serves as a common interface for them
class ICameraParamsProvider {
public:
  virtual ~ICameraParamsProvider() = default;
  virtual uvec2 cameraViewSize() const = 0;
  virtual vec3 cameraPosition() const = 0;
  virtual vec3 cameraLeft() const = 0;
  virtual vec3 cameraRight() const = 0;
  virtual vec3 cameraUp() const = 0;
  virtual vec3 cameraDown() const = 0;
  virtual mat4 cameraViewProjectionMatrix() const = 0;
  virtual mat4 cameraViewMatrix() const = 0;
  virtual mat4 cameraProjectionMatrix() const = 0;
  virtual vec4 cameraViewport() const = 0;
};

#endif //CPPWRAPPER_ICAMERAPARAMSPROVIDER_H
