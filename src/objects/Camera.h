//
// Created by Sidorenko Nikita on 4/3/18.
//

#ifndef CPPWRAPPER_CAMERA_H
#define CPPWRAPPER_CAMERA_H

#include "scene/GameObject.h"
#include "EngMath.h"

class Camera : public GameObject {
public:
  const mat4 &projectionMatrix() const { return _projectionMatrix; }
  const mat4 &viewMatrix() const { return _viewMatrix; }
  const mat4 viewProjectionMatrix() const { return _projectionMatrix * _viewMatrix; }
  const vec4 viewport() const { return _viewport; }
  void postUpdate() override;

protected:
  mat4 _projectionMatrix;
  mat4 _viewMatrix;
  vec4 _viewport;

  float _fow;
  float _aspect; // screen aspect
  unsigned long _mask = ~0ul; // mask to filter gameObjects

protected:
  inline void _updateProjection();
  inline void _updateView();

  void _updateViewport();
};

typedef std::shared_ptr<Camera> CameraPtr;


#endif //CPPWRAPPER_CAMERA_H
