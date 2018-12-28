//
// Created by Sidorenko Nikita on 4/3/18.
//

#include "Camera.h"
#include "EngineMain.h"
#include "render/renderer/Renderer.h"

void Camera::_updateProjection() {
  auto engine = getEngine();
  auto window = engine->window();

  switch (_mode) {
    case Mode::Perspective:
      _projectionMatrix = glm::perspective(glm::radians(_fov), window->aspect(), 0.1f, 100.0f);
      break;

    case Mode::Ortho: {
      float halfHeight = _orthographicSize / 2.0f;
      float halfWidth = window->aspect() * halfHeight;
      _projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f);
      break;
    }
    case Mode::UI:
      float halfHeight = window->height() / 2.0f;
      float halfWidth = window->width() / 2.0f;
      _projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
      break;
  }
}

void Camera::_updateView() {
  _viewMatrix = glm::inverse(transform()->worldMatrix());
}

void Camera::_updateViewport() {
  auto engine = getEngine();
  _viewport = engine->window()->viewport(); // TODO: modify if camera rendertarget support is required
}

void Camera::postUpdate() {
  this->_updateViewport();
  this->_updateProjection();
  this->_updateView();
  _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
  _frustum.calcPlanes(_viewProjectionMatrix);
}
