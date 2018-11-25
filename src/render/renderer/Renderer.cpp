//
// Created by Sidorenko Nikita on 2/18/18.
//

#include <system/Window.h>
#include <glm/gtx/perpendicular.inl>
#include "Renderer.h"
#include "loader/ShaderLoader.h"

#include "EngMath.h"
#include "render/mesh/Mesh.h"
#include "render/shader/Shader.h"
#include "system/Logging.h"
#include "glm/glm.hpp"
#include "scene/Scene.h"
#include"render/material/Material.h"
#include"render/material/MaterialTypes.h"
#include "objects/Camera.h"
#include "render/lighting/LightGrid.h"
#include "render/debug/DebugDraw.h"
#include "render/shader/Uniform.h"
#include "View.h"
#include "utils/Performance.h"

using namespace glm;

Renderer::Renderer() {
  _projectorTextureUniform = UNIFORM_TEXTURE_BLOCKS.at(UniformName::ProjectorTexture);
  _lightGrid = std::make_unique<LightGrid>();
  _uboManager = std::make_unique<UBOManager>();
  _debugDraw = std::make_shared<DebugDraw>();
  _lightGrid->setDebugDraw(_debugDraw);
  _depthPrePassMaterial = std::make_shared<MaterialDepthPrepass>();
}

Renderer::~Renderer() {
  // requires non-inline destructor to make unique_ptr forward declaration work
}

void Renderer::renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform, GLenum mode) {
  glBindVertexArray(mesh->vao());

  if (mesh->hasIndices()) {
    glDrawElements(mode, mesh->indexCount(), GL_UNSIGNED_SHORT, 0);
  } else {
    glDrawArrays(mode, 0, mesh->indexCount());
  }
}

void Renderer::setupAndUploadUBO(RenderOperation *rop) {
  if (rop->material->hasTransformBlock()) {
    UBOStruct::TransformStruct transformStruct;
    transformStruct.transform = rop->modelMatrix;
    transformStruct.normalMatrix = glm::inverseTranspose(rop->modelMatrix);
    rop->material->setTransformBlock(transformStruct);
  }

  _uboManager->setTransformBlock(rop);
}

void Renderer::renderScene(std::shared_ptr<Scene> scene, ViewPtr view) {
  switch (view->mode()) {
    case RenderMode::DepthOnly:
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthFunc(GL_LESS);
      break;

    case RenderMode::Normal:
      glDepthMask(GL_FALSE);
      glEnable(GL_DEPTH_TEST);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthFunc(GL_LEQUAL);
      break;

    case RenderMode::NormalDepthWrite:
      glDepthMask(GL_TRUE);
      glEnable(GL_DEPTH_TEST);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthFunc(GL_LEQUAL);
      break;
  }

  bool needsLighting = true || view->mode() != RenderMode::DepthOnly;

  _clearQueues();

  auto camera = view->camera();

  auto visibleObjects = scene->visibleObjects(camera);
  for (auto &object : visibleObjects) {
    object->render(*this);
  }

  _debugDraw->render(*this);

  _uboManager->map();
  for (auto &queue : _queues) {
    for (auto &rop : queue) {
      setupAndUploadUBO(&rop);
    }
  }
  _uboManager->unmap();

  if (needsLighting) {
    engine::Performance::startTimer(engine::Performance::Entry::LightGrid);
    auto windowSize = camera->cameraViewSize();
    _lightGrid->update(windowSize.x, windowSize.y);

    auto lights = scene->visibleLights(camera);
    _lightGrid->appendLights(lights, camera);
    auto projectors = scene->visibleProjectors(camera);
    _lightGrid->appendProjectors(projectors, camera);

    _lightGrid->upload();
    _lightGrid->bindBufferTextures();
    _uboManager->updateLights(lights);
    _uboManager->updateProjectors(projectors);
    engine::Performance::stopTimer(engine::Performance::Entry::LightGrid);
  }

  _uboManager->setCamera(camera);
  _uboManager->upload(needsLighting);

  _processRenderPipeline(view->mode());
}

void Renderer::_clearQueues() {
  _ropCounter = 0;
  for (auto &queue : _queues) {
    queue.clear();
  }
}

void Renderer::_renderCamera(std::shared_ptr<Scene> scene, std::shared_ptr<ICameraParamsProvider> camera) {

}

void Renderer::_prepareQueues(std::shared_ptr<Scene> scene, CameraPtr camera) {

}

void Renderer::_processRenderPipeline(RenderMode mode) {
  if (_projectorTexture && _projectorTexture->id()) {
    glActiveTexture(GL_TEXTURE0 + _projectorTextureUniform);
    glBindTexture(GL_TEXTURE_2D, _projectorTexture->id());
  }

  bool isDepthOnly = mode == RenderMode::DepthOnly;
  MaterialPtr tempMaterial;

  // Opaque
  auto &opaqueQueue = _queues[(int)RenderQueue::Opaque];
  for (auto &rop : opaqueQueue) {
    if (isDepthOnly) {
      tempMaterial = rop.material;
      rop.material = _depthPrePassMaterial;
      rop.material->setTransformBlock(tempMaterial->getTransformStruct());
    }
    _uboManager->setupForRender(&rop);
    renderMesh(rop.mesh, rop.material, rop.modelMatrix, rop.mode);
    if (isDepthOnly) {
      rop.material = tempMaterial;
    }
  }

  if (isDepthOnly) {
    return;
  }

  // Debug
//  glEnable(GL_PROGRAM_POINT_SIZE);
//  glDisable(GL_DEPTH_TEST);
  auto &debugQueue = _queues[(int)RenderQueue::Debug];
  for (auto &rop : debugQueue) {
    _uboManager->setupForRender(&rop);
    renderMesh(rop.mesh, rop.material, rop.modelMatrix, rop.mode);
  }
//  glDisable(GL_PROGRAM_POINT_SIZE);
//  glEnable(GL_DEPTH_TEST);
}

void Renderer::addRenderOperation(RenderOperation &rop, RenderQueue renderQueue) {
  if (!rop.material || !rop.mesh) {
    ENGLog("Trying to add non-renderable rop");
    return;
  }

  auto &queue = _queues[(int)renderQueue];
  rop.index = _ropCounter++;
  queue.push_back(rop);
}

void Renderer::renderPrepare() {
  _uboManager->swap();
}
