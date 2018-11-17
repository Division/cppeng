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
#import "objects/LightObject.h"
#import "objects/Projector.h"

const unsigned int MAX_LIGHTS = 1000;

UBOManager::UBOManager() {
  auto lightMaxSize = sizeof(UBOStruct::Light) * MAX_LIGHTS;
  auto projectorMaxSize = sizeof(UBOStruct::Projector) * MAX_LIGHTS;

  _transform = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
  _light = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, lightMaxSize);
  _projector = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, projectorMaxSize);
  _camera = std::make_shared<SwappableVertexBufferObject>(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
}

void UBOManager::updateLights(const std::vector<LightObjectPtr> *lights) {
  auto alignBytes = 0u;
  auto buffer = _light->current();

  // note that elements in lights array may be null
  for (auto &light : *lights) {
    if (!light) {
      continue;
    }

    UBOStruct::Light lightData = light->getLightStruct();
    buffer->appendData((void *) &lightData, sizeof(lightData), alignBytes);
  }
}

void UBOManager::updateProjectors(const std::vector<ProjectorPtr> *projectors) {
  auto alignBytes = 0u;
  auto buffer = _projector->current();

  // note that elements in projectors array may be null
  for (auto &projector : *projectors) {
    if (!projector) {
      continue;
    }

    UBOStruct::Projector projectorData = projector->getProjectorStruct();
    buffer->appendData((void *) &projectorData, sizeof(projectorData), alignBytes);
  }
}

void UBOManager::setCamera(CameraPtr camera) {
  UBOStruct::Camera cameraData;
  cameraData.position = camera->transform()->worldPosition();
  cameraData.screenSize = camera->screenSize();
  cameraData.viewMatrix = camera->viewMatrix();
  cameraData.projectionMatrix = camera->projectionMatrix();

  auto alignBytes = (unsigned int)engine::GLCaps::uboOffsetAlignment();
  auto buffer = _camera->current();
  buffer->appendData((void *) &cameraData, sizeof(cameraData), alignBytes);
}

void UBOManager::processMeterialBindings(RenderOperation *rop) {
  MaterialPtr material = rop->material;
  auto buffer = _transform->current();

  if (material->hasTransformBlock()) {
    auto &transformStruct = material->getTransformStruct();
    auto alignBytes = (unsigned int)engine::GLCaps::uboOffsetAlignment();
    auto offset = (unsigned int)buffer->appendData((void *) &transformStruct, sizeof(transformStruct), alignBytes);
    rop->transformBlockOffset = offset;
  }
}

void UBOManager::swap() {
  _transform->swap();
  _transform->current()->resize(0);

  _light->swap();
  _light->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Light, _light->current()->vbo());

  _projector->swap();
  _projector->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Projector, _projector->current()->vbo());

  _camera->swap();
  _camera->current()->resize(0);
  glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)UniformBlockName::Camera, _camera->current()->vbo());
}

void UBOManager::upload() {
  _transform->current()->upload();
  _light->current()->upload();
  _projector->current()->upload();
  _camera->current()->upload();
}

// TODO: move out of this class
void UBOManager::setupForRender(RenderOperation *rop) {
  MaterialPtr material = rop->material;
  material->shader()->bind();
  material->uploadBindings();
  material->activateTextures();

  if (material->hasTransformBlock()) {
    auto offset = rop->transformBlockOffset;
    auto size = sizeof(UBOStruct::TransformStruct);
    auto slot = (GLuint)UniformBlockName::Transform;
    auto vbo = _transform->current()->vbo();
    glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, offset, size);
  } else {
    ENGLog("No transform");
  }
}
