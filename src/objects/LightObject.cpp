//
// Created by Sidorenko Nikita on 4/18/18.
//

#include "LightObject.h"
#include <memory>
#include "EngineMain.h"
#include "render/debug/DebugDraw.h"
#include "render/renderer/SceneRenderer.h"
#include "utils/Math.h"

void LightObject::_updateAttenuation() {
  _squareAttenuation = 1.0f / (_radius * _radius * _lightCutoff);
}

void LightObject::_updateRadius() {
  _radius = fabs(( -_linearAttenuation - std::sqrtf(_linearAttenuation * _linearAttenuation - 4 * _squareAttenuation * (1 - 1 / _lightCutoff))) / (2 * _squareAttenuation));
}

LightObject::LightObject() : GameObject() {
  _cullingData.type = CullingData::Type::Sphere;
}

UBOStruct::Light LightObject::getLightStruct() const {
  UBOStruct::Light result;

  result.position = transform()->worldPosition();
  result.squareAttenuation = squareAttenuation();
  result.linearAttenuation = linearAttenuation();
  result.color = color();

  // Assign for point light as well because point light shadow is calculated like spotlight shadow
  result.coneAngle = cosf(RAD(_coneAngle) / 2.0f);
  result.direction = glm::normalize(transform()->forward());

  if (castShadows()) {
    result.shadowmapScale = vec2(_viewport.z, _viewport.w) / SceneRenderer::shadowAtlasSize();
    result.shadowmapOffset= vec2(_viewport.x, _viewport.y) / SceneRenderer::shadowAtlasSize();
    result.projectionMatrix = cameraViewProjectionMatrix();
  } else {
    result.shadowmapScale = vec2(0, 0);
  }

  return result;
}

void LightObject::enableDebug() {
  if (_debugMesh) {
    return;
  }

  _debugMesh = std::make_shared<Mesh>();
  switch (_type) {
    case LightObjectType::Point:
      MeshGeneration::generateSphere(_debugMesh, 10, 10, 0.2);
      break;

    case LightObjectType::Spot:
      float spotRadius = getSpotRadius(1);
      MeshGeneration::generateCone(_debugMesh, 1, spotRadius);
  }
  _debugMesh->createBuffer();

  _debugMaterial = std::make_shared<MaterialSingleColor>();
}

void LightObject::render(IRenderer &renderer) {
  if (!_debugMesh || !_debugMaterial) {
    return;
  }

  _debugMaterial->color(vec4(color(), 1));
  RenderOperation rop;
  rop.mesh = _debugMesh;
  rop.material = _debugMaterial;
  rop.modelMatrix = transform()->worldMatrix();
  rop.debugInfo = name();
  renderer.addRenderOperation(rop, RenderQueue::Opaque);
//  getEngine()->debugDraw()->drawAABB(this->bounds(), vec4(this->color(), 1));
}

float LightObject::getSpotRadius(float height) {
  if (_type == LightObjectType::Spot) {
    return tanf(_coneAngle / 2.0f * (float)M_PI / 180) * height;
  }

  return 0;
}

void LightObject::attenuation(float linear, float square) {
  _linearAttenuation = linear;
  _squareAttenuation = square;
  _updateRadius();
}

void LightObject::postUpdate() {
  // TODO: add directional support
  // Shadow maps are square, so aspect is 1
  _projectionMatrix = glm::perspective(glm::radians(_coneAngle), 1.0f, _zMin, _radius);
  _viewMatrix = glm::inverse(transform()->worldMatrix());
  _frustum.calcPlanes(_projectionMatrix * _viewMatrix);

  auto position = transform()->worldPosition();
  if (_type == LightObjectType::Spot) {
    _cullingData.sphere = boundingSphereForFrustum(1, 1, 0, _radius, RAD(_coneAngle));
    _cullingData.sphere.position += position;
  } else {
    _cullingData.sphere.position = position;
    _cullingData.sphere.radius = _radius;
  };
}
