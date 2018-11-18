//
// Created by Sidorenko Nikita on 11/15/18.
//

#include "Projector.h"
#include "render/shader/UniformBufferStruct.h"
#include "engine/EngineMain.h"
#include "EngMath.h"

AABB Projector::bounds() {
  auto position = transform()->worldPosition();
  return AABB::fromSphere(position, 10);
}


void Projector::postUpdate() {
  _viewProjection = _getProjection() * glm::inverse(transform()->worldMatrix());
}

mat4 Projector::_getProjection() {
  if (_isOrthographic) {
    float halfHeight = _orthographicSize / 2.0f;
    float halfWidth = _aspect * halfHeight;
    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, _zNear, _zFar);
  } else {
    return glm::perspective(RAD(_fov), _aspect, _zNear, _zFar);
  }
}

UBOStruct::Projector Projector::getProjectorStruct() const {
  UBOStruct::Projector result;

  result.position = transform()->worldPosition();
  result.squareAttenuation = _squareAttenuation;
  result.linearAttenuation = _linearAttenuation;
  result.color = _color;
  result.scale = vec2(_spriteBounds.width, _spriteBounds.height);
  result.offset = vec2(_spriteBounds.x, _spriteBounds.y);
  result.projectionMatrix = _viewProjection;

  return result;
}

void Projector::render(IRenderer &renderer) {
  if (_debugEnabled) {
    auto debugDraw = getEngine()->debugDraw();
    debugDraw->drawFrustum(_viewProjection, glm::vec4(1,1,1,1));
  }
}

// Fills the result vector with world-space frustum edge vertices
void Projector::getEdgePoints (std::vector<vec3> &outEdgePoints) {
  outEdgePoints.resize(8);

  auto inv = glm::inverse(_viewProjection);
  vec4 quad1[4] = {
      inv * vec4(1, -1, -1, 1),
      inv * vec4(-1, -1, -1, 1),
      inv * vec4(-1, 1, -1, 1),
      inv * vec4(1, 1, -1, 1),
  };

  vec4 quad2[4] = {
      inv * vec4(1, -1, 1, 1),
      inv * vec4(-1, -1, 1, 1),
      inv * vec4(-1, 1, 1, 1),
      inv * vec4(1, 1, 1, 1),
  };

  for (int i = 0; i < 8; i++) {
    auto &quad = i < 4 ? quad1 : quad2;
    int index = i % 4;
    outEdgePoints[i] = vec3(quad[index] / quad[index].w);
  }
}

void Projector::attenuation(float linear, float square) {
  _linearAttenuation = linear;
  _squareAttenuation = square;
}