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
#include "render/shading/LightGrid.h"
#include "render/debug/DebugDraw.h"
#include "render/shader/Uniform.h"
#include "RenderPass.h"
#include "utils/Performance.h"
#include "objects/LightObject.h"
#include "objects/Projector.h"

using namespace glm;

Renderer::Renderer(DebugDrawPtr debugDraw) {
  _projectorTextureUniform = UNIFORM_TEXTURE_BLOCKS.at(UniformName::ProjectorTexture);
  _lightGrid = std::make_unique<LightGrid>();
  _uboManager = std::make_unique<UBOManager>();
  _debugDraw = debugDraw;
  _lightGrid->setDebugDraw(_debugDraw);
}

Renderer::~Renderer() {
  // requires non-inline destructor to make unique_ptr forward declaration work
}

void Renderer::renderMesh(MeshPtr mesh, GLenum mode) {
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
//    transformStruct.normalMatrix = glm::inverseTranspose(rop->modelMatrix);
    rop->material->setTransformBlock(transformStruct);
  }

  if (rop->isSkinning) {
    _skinningRops.push_back(rop);
  }

  _uboManager->setTransformBlock(rop);
}

void Renderer::_uploadSkinning(RenderOperation *rop) {
  _uboManager->setSkinningMatrices(rop);
}

// Executed once per frame. Upload most of the UBO.
void Renderer::setupBuffers(ScenePtr &scene, ICameraParamsProviderPtr &camera, ICameraParamsProviderPtr &camera2D) {
  _uboManager->swap();

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

  if (camera2D) {
    _uboManager->appendCamera(std::static_pointer_cast<ICameraParamsProvider>(camera2D));
  }
  _uboManager->appendCamera(std::static_pointer_cast<ICameraParamsProvider>(camera));

  for (auto &light : lights) {
    if (light->castShadows()) {
      _uboManager->appendCamera(std::static_pointer_cast<ICameraParamsProvider>(light));
    }
  }

  for (auto &projector: projectors) {
    if (projector->castShadows()) {
      _uboManager->appendCamera(std::static_pointer_cast<ICameraParamsProvider>(projector));
    }
  }

  _uboManager->upload(true);
}

// Called for every camera
void Renderer::populateQueues(std::shared_ptr<Scene> scene, ICameraParamsProviderPtr camera) {

  auto visibleObjects = scene->visibleObjects(camera);
  for (auto &object : visibleObjects) {
    object->render(*this);
  }

  _skinningRops.clear();

  _uboManager->map();
  for (auto &queue : _queues) {
    for (auto &rop : queue) {
      setupAndUploadUBO(&rop);
    }
  }
  _uboManager->unmap();

  _uboManager->mapSkinning();
  for (auto rop : _skinningRops) {
    _uploadSkinning(rop);
  }
  _uboManager->unmapSkinning();
}

void Renderer::renderScene(RenderPassPtr view) {
  switch (view->mode()) {
    case RenderMode::DepthOnly:
      glDepthMask(GL_TRUE);
//      glClear(GL_DEPTH_BUFFER_BIT);
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

    case RenderMode::UI:
      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      break;
  }

  auto camera = view->camera();
  _uboManager->activateCamera(camera->cameraIndex());

  auto viewport = camera->cameraViewport();
  glViewport((int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);
  _processRenderPipeline(view->mode());
}

void Renderer::clearQueues() {
  _ropCounter = 0;
  for (auto &queue : _queues) {
    queue.clear();
  }
}

void Renderer::_processRenderPipeline(RenderMode mode) {
  // For UI pass just render everything and exit function
  if (mode == RenderMode::UI) {
    glDisable(GL_CULL_FACE);
    auto &uiQueue = _queues[(int)RenderQueue::UI];
    for (auto &rop : uiQueue) {
      _uboManager->setupForRender(&rop, mode);
      renderMesh(rop.mesh, rop.mode);
    }
    glEnable(GL_CULL_FACE);
    return;
  }
  // ---------------

  // Next, binding projector texture
  if (_projectorTexture && _projectorTexture->id()) {
    glActiveTexture(GL_TEXTURE0 + _projectorTextureUniform);
    glBindTexture(GL_TEXTURE_2D, _projectorTexture->id());
  }
  // ---------------

  bool isDepthOnly = mode == RenderMode::DepthOnly;
  MaterialPtr tempMaterial;

  // Opaque
  auto &opaqueQueue = _queues[(int)RenderQueue::Opaque];
  for (auto &rop : opaqueQueue) {
    _uboManager->setupForRender(&rop, mode);
    renderMesh(rop.mesh, rop.mode);
  }
  // ---------------

  if (isDepthOnly) { // Debug and transparency are skipped for depth only
    return;
  }

  // Debug
  auto &debugQueue = _queues[(int)RenderQueue::Debug];
  glDisable(GL_DEPTH_TEST);
  for (auto &rop : debugQueue) {
    _uboManager->setupForRender(&rop, mode);
    renderMesh(rop.mesh, rop.mode);
  }
  glEnable(GL_DEPTH_TEST);
  // ---------------
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

