//
// Created by Sidorenko Nikita on 2/18/18.
//

#include <system/Window.h>
#include "Renderer.h"
#include "loader/ShaderLoader.h"

#include "EngMath.h"
#include "render/mesh/Mesh.h"
#include "render/shader/Shader.h"
#include "system/Logging.h"
#include "glm/glm.hpp"
#include "scene/Scene.h"
#include"render/material/Material.h"
#include "objects/Camera.h"
#include "render/lighting/LightGrid.h"
#include "render/debug/DebugDraw.h"
#include "render/shader/Uniform.h"
#include "View.h"

using namespace glm;

Renderer::Renderer() {
  _projectorTextureUniform = UNIFORM_TEXTURE_BLOCKS.at(UniformName::ProjectorTexture);
  _lightGrid = std::make_unique<LightGrid>();
  _uboManager = std::make_unique<UBOManager>();
  _debugDraw = std::make_shared<DebugDraw>();
  _lightGrid->setDebugDraw(_debugDraw);
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
  _renderCamera(scene, view->camera());
}

void Renderer::_clearQueues() {
  _ropCounter = 0;
  for (auto &queue : _queues) {
    queue.clear();
  }
}

void Renderer::_renderCamera(std::shared_ptr<Scene> scene, std::shared_ptr<ICameraParamsProvider> camera) {
  _clearQueues();

  auto visibleObjects = scene->visibleObjects(camera);
  for (auto &object : visibleObjects) {
    object->render(*this);
  }

  _debugDraw->render(*this);
  _uboManager->swap();

  for (auto &queue : _queues) {
    for (auto &rop : queue) {
      setupAndUploadUBO(&rop);
    }
  }

  auto windowSize = camera->cameraViewSize();
  _lightGrid->update(windowSize.x, windowSize.y);

  auto lights = scene->visibleLights(camera);
  _lightGrid->appendLights(lights, camera);
  auto projectors = scene->visibleProjectors(camera);
  _lightGrid->appendProjectors(projectors, camera);

  _lightGrid->upload();
  _lightGrid->bindBufferTextures();

  _uboManager->setCamera(camera);
  _uboManager->updateLights(lights);
  _uboManager->updateProjectors(projectors);
  _uboManager->upload();

  _processRenderPipeline();
}

void Renderer::_prepareQueues(std::shared_ptr<Scene> scene, CameraPtr camera) {

}

void Renderer::_processRenderPipeline() {
  if (_projectorTexture && _projectorTexture->id()) {
    glActiveTexture(GL_TEXTURE0 + _projectorTextureUniform);
    glBindTexture(GL_TEXTURE_2D, _projectorTexture->id());
  }

  // Opaque
  auto &opaqueQueue = _queues[(int)RenderQueue::Opaque];
  for (auto &rop : opaqueQueue) {
    _uboManager->setupForRender(&rop);

    renderMesh(rop.mesh, rop.material, rop.modelMatrix, rop.mode);
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
