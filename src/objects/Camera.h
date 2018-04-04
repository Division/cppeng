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
  void postUpdate() override;

protected:
  mat4 _projectionMatrix;
  mat4 _viewMatrix;

  float _fow;
  float _aspect; // screen aspect
  unsigned long _mask = ~0ul; // mask to filter gameObjects

protected:
  inline void _updateProjection();
  inline void _updateView();
};

typedef std::shared_ptr<Camera> CameraPtr;


#endif //CPPWRAPPER_CAMERA_H
