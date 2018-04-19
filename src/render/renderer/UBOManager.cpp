//
// Created by Sidorenko Nikita on 4/13/18.
//

#include "UBOManager.h"
#include "EngineGL.h"
#include "system/Logging.h"
#include <memory>
#include <vector>

UBOManager::UBOManager() {
  _transform = std::make_unique<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
  _light = std::make_unique<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
}

void UBOManager::updateLights(const std::vector<LightObjectPtr> *lights) {
  auto alignBytes = (unsigned int)engine::GLCaps::uboOffsetAlignment();
  auto buffer = _light->current();

  for (auto &light : *lights) {
    UBOStruct::Light lightData;
    if (light) {
      lightData = light->getLightStruct();
    }

    buffer->appendData((void *) &lightData, sizeof(lightData), alignBytes);
  }
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
}

void UBOManager::upload() {
  _transform->current()->upload();
  _light->current()->upload();
}

void UBOManager::setupForRender(MaterialPtr material) {
  material->shader()->bind();
  material->uploadBindings();

  if (material->hasTransformBlock()) {
//    GLint bufSize;
//    glBindBuffer(GL_UNIFORM_BUFFER, _transform->current()->vbo());
//    glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &bufSize);
//    ENGLog("B %i, VBO %i, OFFS %i, size %i", (GLuint)UniformBlockName::Transform,  _transform->current()->vbo(), material->getTransformBlockOffset(), bufSize);

    auto offset = material->getTransformBlockOffset();
    auto size = sizeof(UBOStruct::TransformStruct);
    auto slot = (GLuint)UniformBlockName::Transform;
    auto vbo = _transform->current()->vbo();
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, offset, size);
  }
}
