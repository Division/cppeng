//
// Created by Sidorenko Nikita on 4/18/18.
//

#include "LightObject.h"
#include <memory>

void LightObject::_updateAttenuation() {
  _squareAttenuation = 1.0f / (_radius * _radius * _lightCutoff);
}

void LightObject::_updateRadius() {
  _radius = sqrtf(1.0f / (_squareAttenuation * _lightCutoff));
}

LightObject::LightObject() : GameObject() {
  radius(5);
}

UBOStruct::Light LightObject::getLightStruct() const {
  UBOStruct::Light result;

  result.position = transform()->worldPosition();
  result.attenuation = squareAttenuation();
  result.color = color();

  return result;
}

void LightObject::enableDebug() {
  if (_debugMesh) {
    return;
  }

  _debugMesh = std::make_shared<Mesh>();
  MeshGeneration::generateSphere(_debugMesh, 10, 10, 0.2);
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
}