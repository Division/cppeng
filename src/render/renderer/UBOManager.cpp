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

  _transform = std::make_shared<MultiVertexBufferObject>(
    GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW,
    engine::GLCaps::maxUBOSize(),
    engine::GLCaps::uboOffsetAlignment()
  );

  _skinningMatrices = std::make_shared<MultiVertexBufferObject>(
      GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW,
      engine::GLCaps::maxUBOSize(),
      engine::GLCaps::uboOffsetAlignment()
  );

  _light = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, lightMaxSize);
  _projector = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, projectorMaxSize);
  _camera = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
}

void UBOManager::updateLights(const std::vector<LightObjectPtr> &lights) {
  auto alignBytes = 0u;
  auto buffer = _light->current();

  // note that elements in lights array may be null
  for (auto &light : lights) {
    if (!light) {
      continue;
    }

    UBOStruct::Light lightData = light->getLightStruct();
    buffer->appendData((void *) &lightData, sizeof(lightData), alignBytes);
  }
}

void UBOManager::updateProjectors(const std::vector<ProjectorPtr> &projectors) {
  auto alignBytes = 0u;
  auto buffer = _projector->current();

  // note that elements in projectors array may be null
  for (auto &projector : projectors) {
    if (!projector) {
      continue;
    }

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

void UBOManager::setTransformBlock(RenderOperation *rop) {
  MaterialPtr material = rop->material;
  if (material->hasTransformBlock()) {
    auto &transformStruct = material->getTransformStruct();
    auto address = _transform->appendData((void *) &transformStruct, sizeof(transformStruct));
    rop->transformBlockOffset = address;
  }
}

void UBOManager::swap() {
  _transform->swapBuffers();

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

  bool isSkinning = false;
  bool isDepthOnly = mode == RenderMode::DepthOnly;

  ShaderPtr shader;

  if (isDepthOnly) {
    shader = isSkinning ? material->shaderDepthOnlySkinning() : material->shaderDepthOnly();
  } else {
    shader = isSkinning ? material->shaderSkinning() : material->shader();
  }

  shader->bind();

  // For now depth only pass doesn't need shader bingings
  if (!isDepthOnly) {
    material->uploadBindings(shader);
    material->activateTextures();
  }

  if (material->hasTransformBlock()) {
    auto address = rop->transformBlockOffset;
    auto size = sizeof(UBOStruct::TransformStruct);
    auto slot = (GLuint)UniformBlockName::Transform;
    auto vbo = _transform->getVBO(address.index);
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, address.offset, size);
  } else {
    ENGLog("No transform");
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
  _transform->map();
}

void UBOManager::unmap() {
  _transform->unmap();
}
