//
// Created by Sidorenko Nikita on 4/13/18.
//

#include "UBOManager.h"
#include "EngineGL.h"
#include "system/Logging.h"
#include <memory>
#include <vector>
#include "render/buffer/VertexBufferObject.h"
#include "objects/Camera.h"
#include "EngineGL.h"

const unsigned int MAX_LIGHTS = 1000;

UBOManager::UBOManager() {
  unsigned int lightMaxSize = sizeof(UBOStruct::Light) * MAX_LIGHTS;

  _transform = std::make_unique<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
  _light = std::make_unique<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, lightMaxSize);
  _camera = std::make_unique<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
}

void UBOManager::updateLights(const std::vector<LightObjectPtr> *lights) {

  auto alignBytes = 0;
  auto buffer = _light->current();

  for (auto &light : *lights) {
    UBOStruct::Light lightData;
    if (light) {
      lightData = light->getLightStruct();
    }

    buffer->appendData((void *) &lightData, sizeof(lightData), alignBytes);
  }
}

void UBOManager::setCamera(CameraPtr camera) {
  UBOStruct::Camera cameraData;
  cameraData.position = camera->transform()->worldPosition();
  cameraData.screenSize = camera->screenSize();

  auto alignBytes = (unsigned int)engine::GLCaps::uboOffsetAlignment();
  auto buffer = _camera->current();
  buffer->appendData((void *) &cameraData, sizeof(cameraData), alignBytes);
}

void UBOManager::processMeterialBindings(MaterialPtr material) {
  auto buffer = _transform->current();

  if (material->hasTransformBlock()) {
    auto &transformStruct = material->getTransformStruct();
    auto alignBytes = (unsigned int)engine::GLCaps::uboOffsetAlignment();
    auto offset = (unsigned int)buffer->appendData((void *) &transformStruct, sizeof(transformStruct), alignBytes);
    material->setTransformBlockOffset(offset);
  }
}

void UBOManager::swap() {
  _transform->swap();
  _transform->current()->resize(0);

  _light->swap();
  _light->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Light, _light->current()->vbo());

  _camera->swap();
  _camera->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Camera, _camera->current()->vbo());
}

void UBOManager::upload() {
  _transform->current()->upload();
  _light->current()->upload();
  _camera->current()->upload();
}

void UBOManager::setupForRender(MaterialPtr material) {
  material->shader()->bind();
  material->uploadBindings();
  material->activateTextures();

  if (material->hasTransformBlock()) {
    auto offset = material->getTransformBlockOffset();
    auto size = sizeof(UBOStruct::TransformStruct);
    auto slot = (GLuint)UniformBlockName::Transform;
    auto vbo = _transform->current()->vbo();
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, offset, size);
  } else {
    ENGLog("No transform");
  }
}
