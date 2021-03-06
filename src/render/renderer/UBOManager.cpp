//
// Created by Sidorenko Nikita on 4/13/18.
//

#include "UBOManager.h"
#include "EngineGL.h"
#include "system/Logging.h"
#include <memory>
#include <vector>
#include "render/buffer/MultiVertexBufferObject.h"
#include "render/buffer/VertexBufferObject.h"
#include "objects/Camera.h"
#include "EngineGL.h"
#include "objects/LightObject.h"
#include  "objects/Projector.h"
#include "render/renderer/ICameraParamsProvider.h"

const unsigned int MAX_LIGHTS = 1000;

UBOManager::UBOManager() {
  auto lightMaxSize = sizeof(UBOStruct::Light) * MAX_LIGHTS;
  auto projectorMaxSize = sizeof(UBOStruct::Projector) * MAX_LIGHTS;

  _objectParams = std::make_shared<MultiVertexBufferObject>(
    GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW,
    engine::GLCaps::maxUBOSize(),
    engine::GLCaps::uboOffsetAlignment()
  );

  _skinningMatrices = std::make_shared<MultiVertexBufferObject>(
    GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW,
    engine::GLCaps::maxUBOSize(),
    engine::GLCaps::uboOffsetAlignment()
  );

  _light = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, (unsigned int)lightMaxSize);
  _projector = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, (unsigned int)projectorMaxSize);
  _camera = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
}

void UBOManager::updateLights(const std::vector<LightObjectPtr> &lights) {
  auto alignBytes = 0u;
  auto buffer = _light->current();

  // note that elements in lights array may be null
  for (auto &light : lights) {
    UBOStruct::Light lightData = light->getLightStruct();
    buffer->appendData((void *) &lightData, sizeof(lightData), alignBytes);
  }
}

void UBOManager::updateProjectors(const std::vector<ProjectorPtr> &projectors) {
  auto alignBytes = 0u;
  auto buffer = _projector->current();

  // note that elements in projectors array may be null
  for (auto &projector : projectors) {
    UBOStruct::Projector projectorData = projector->getProjectorStruct();
    buffer->appendData((void *) &projectorData, sizeof(projectorData), alignBytes);
  }
}

int UBOManager::appendCamera(std::shared_ptr<ICameraParamsProvider> camera) {
  UBOStruct::Camera cameraData;
  cameraData.position = camera->cameraPosition();
  cameraData.screenSize = camera->cameraViewSize();
  cameraData.viewMatrix = camera->cameraViewMatrix();
  cameraData.projectionMatrix = camera->cameraProjectionMatrix();

  auto alignBytes = (unsigned int)engine::GLCaps::uboOffsetAlignment();
  auto buffer = _camera->current();
  auto index = buffer->appendData((void *) &cameraData, sizeof(cameraData), alignBytes);
  camera->cameraIndex((unsigned int)index);
  return index;
}

void UBOManager::setObjectParamsBlock(RenderOperation *rop) {
  if (rop->objectParams) {
    auto address = _objectParams->appendData((void *)rop->objectParams, sizeof(UBOStruct::ObjectParams));
    rop->objectParamsBlockOffset = address;
  }
}

void UBOManager::setSkinningMatrices(RenderOperation *rop) {
  MaterialPtr material = rop->material;
  auto skinningMatrices = rop->skinningMatrices;
  auto address = _skinningMatrices->appendData((void *)skinningMatrices, sizeof(UBOStruct::SkinningMatrices));
  rop->skinningOffset = address;
}

void UBOManager::swap() {
  _objectParams->swapBuffers();
  _skinningMatrices->swapBuffers();

  _light->swap();
  _light->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Light, _light->current()->vbo());

  _projector->swap();
  _projector->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Projector, _projector->current()->vbo());

  _camera->swap();
  _camera->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Camera, _camera->current()->vbo());

  _activeCameraOffset = -1;
}

void UBOManager::upload(bool includeLighting) {
  if (includeLighting) {
    _light->current()->upload();
    _projector->current()->upload();
  }

  _camera->current()->upload();
}

void UBOManager::setupForRender(RenderOperation *rop, RenderMode mode) {
  MaterialPtr material = rop->material;

  bool isSkinning = (bool)rop->skinningMatrices;
  bool isDepthOnly = mode == RenderMode::DepthOnly;

  ShaderPtr shader;

  if (isDepthOnly) {
    shader = isSkinning ? material->shaderDepthOnlySkinning() : material->shaderDepthOnly();
  } else {
    shader = isSkinning ? material->shaderSkinning() : material->shader();
  }

  shader->bind();

  // depth only pass doesn't need material bindings (e.g. textures)
  if (!isDepthOnly) {
    material->uploadBindings(shader);
    material->activateTextures();
  }

  if (rop->objectParams) {
    auto address = rop->objectParamsBlockOffset;
    auto size = sizeof(UBOStruct::ObjectParams);
    auto slot = (GLuint)UniformBlockName::ObjectParams;
    auto vbo = _objectParams->getVBO(address.index);
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, address.offset, size);
  } else {
    ENGLog("No transform");
  }

  if (rop->skinningMatrices) {
    auto address = rop->skinningOffset;
    auto size = sizeof(UBOStruct::SkinningMatrices);
    auto slot = (GLuint)UniformBlockName::SkinningMatrices;
    auto vbo = _skinningMatrices->getVBO(address.index);
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, address.offset, size);
  }
}

void UBOManager::activateCamera(unsigned int offset) {
  if (_activeCameraOffset != offset) {
    _activeCameraOffset = offset;
    auto size = sizeof(UBOStruct::Camera);
    auto slot = (GLuint)UniformBlockName::Camera;
    auto vbo = _camera->current()->vbo();
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, offset, size);
  }
}

void UBOManager::map() {
  _objectParams->map();
}

void UBOManager::unmap() {
  _objectParams->unmap();
}

void UBOManager::mapSkinning() {
  _skinningMatrices->map();
}

void UBOManager::unmapSkinning() {
  _skinningMatrices->unmap();
}
