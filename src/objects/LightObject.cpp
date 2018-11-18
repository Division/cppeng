//
// Created by Sidorenko Nikita on 4/18/18.
//

#include "LightObject.h"
#include <memory>
#include "engine/EngineMain.h"
#include "render/debug/DebugDraw.h"

void LightObject::_updateAttenuation() {
  _squareAttenuation = 1.0f / (_radius * _radius * _lightCutoff);
}

void LightObject::_updateRadius() {
  _radius = fabs(( -_linearAttenuation - std::sqrtf(_linearAttenuation * _linearAttenuation - 4 * _squareAttenuation * (1 - 1 / _lightCutoff))) / (2 * _squareAttenuation));
}

LightObject::LightObject() : GameObject() {
  _updateRadius();
}

UBOStruct::Light LightObject::getLightStruct() const {
  UBOStruct::Light result;

  result.position = transform()->worldPosition();
  result.squareAttenuation = squareAttenuation();
  result.linearAttenuation = linearAttenuation();
  result.color = color();

  switch (_type) {
    case LightObjectType::Point:
      result.coneAngle = 0;
      break;

    case LightObjectType::Spot:
      result.coneAngle = RAD(_coneAngle) / 2.0f;
      result.direction = glm::normalize(transform()->forward());
      break;
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
  getEngine()->debugDraw()->drawAABB(this->bounds(), vec4(this->color(), 1));
}

float LightObject::getSpotRadius(float height) {
  if (_type == LightObjectType::Spot) {
    return tanf(_coneAngle / 2.0f * (float)M_PI / 180) * height;
  }

  return 0;
}

AABB LightObject::bounds() {
  if (_type == LightObjectType::Spot) {
//    float spotRadius = getSpotRadius(_radius);
//    vec3 left = transform()->left();
    auto position = transform()->worldPosition();
    return AABB::fromSphere(position, _radius);
  } else {
    auto position = transform()->worldPosition();
    return AABB::fromSphere(position, _radius);
  };
}

void LightObject::attenuation(float linear, float square) {
  _linearAttenuation = linear;
  _squareAttenuation = square;
  _updateRadius();
}
