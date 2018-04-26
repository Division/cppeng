//
// Created by Sidorenko Nikita on 4/3/18.
//

#include "Camera.h"
#include "engine/EngineMain.h"
#include "render/renderer/Renderer.h"

void Camera::_updateProjection() {
  _projectionMatrix = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 1000.0f);
}

void Camera::_updateView() {
  _viewMatrix = glm::inverse(transform()->worldMatrix());
}

void Camera::_updateViewport() {
  auto engine = getEngine();
  _viewport = engine->renderer()->viewport();
}

void Camera::postUpdate() {
  this->_updateViewport();
  this->_updateProjection();
  this->_updateView();
}
