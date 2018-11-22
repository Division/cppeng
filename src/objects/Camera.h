//
// Created by Sidorenko Nikita on 4/3/18.
//

#ifndef CPPWRAPPER_CAMERA_H
#define CPPWRAPPER_CAMERA_H

#include "scene/GameObject.h"
#include "EngMath.h"
#include "render/renderer/ICameraParamsProvider.h"

// Right now only one camera's render target can only be the main framebuffer
class Camera : public GameObject, public ICameraParamsProvider {
public:
  const mat4 &projectionMatrix() const { return _projectionMatrix; }
  const mat4 &viewMatrix() const { return _viewMatrix; }
  const mat4 viewProjectionMatrix() const { return _projectionMatrix * _viewMatrix; }
  const vec4 viewport() const { return _viewport; }
  const uvec2 screenSize() const { return uvec2(_viewport.z, _viewport.w); }
  void postUpdate() override;

  // ICameraParamsProvider
  uvec2 cameraViewSize() const override { return  screenSize(); }
  vec3 cameraPosition() const override { return transform()->worldPosition(); }
  mat4 cameraViewProjectionMatrix() const override { return viewProjectionMatrix(); }
  vec3 cameraLeft() const override { return transform()->left(); }
  vec3 cameraRight() const override { return transform()->right(); }
  vec3 cameraUp() const override { return transform()->up(); }
  vec3 cameraDown() const override { return transform()->down(); }
  mat4 cameraViewMatrix() const override { return viewMatrix(); }
  mat4 cameraProjectionMatrix() const override { return projectionMatrix(); }
  vec4 cameraViewport() const override { return viewport(); }

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
