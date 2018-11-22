//
// Created by Sidorenko Nikita on 4/3/18.
//

#include "Camera.h"
#include "EngineMain.h"
#include "render/renderer/Renderer.h"

void Camera::_updateProjection() {
  auto engine = getEngine();
  auto window = engine->window();
  _projectionMatrix = glm::perspective(glm::radians(45.f), window->aspect(), 0.1f, 1000.0f);
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
}
