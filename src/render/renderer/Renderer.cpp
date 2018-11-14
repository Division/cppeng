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

using namespace glm;

Renderer::Renderer(Window *window) {
  _uboManager = std::make_unique<UBOManager>();
  _lightGrid = std::make_unique<LightGrid>();
  _debugDraw = std::make_shared<DebugDraw>();
  _lightGrid->setDebugDraw(_debugDraw);
  _window = window;
}

Renderer::~Renderer() {
  // requires non-inline destructor to make unique_ptr forward declaration work
}

void Renderer::setupShaders() {
  _generator.setupTemplates();
}

ShaderPtr Renderer::getShaderWithCaps (ShaderCapsSetPtr caps) const {
  ShaderPtr result;

  auto iterator = _shaders.find(caps->getBitmask());
  if (iterator == _shaders.end()) {
    std::string shaderSource = _generator.generateShaderSource(caps);
    std::stringstream stream;
    stream.str(shaderSource);
    std::string vertexSource;
    std::string fragmentSource;

    loader::loadShader(stream, &vertexSource, &fragmentSource);
    result = ShaderPtr(new Shader(vertexSource, fragmentSource));
    _shaders[caps->getBitmask()] = result;
  } else {
    result = iterator->second;
  }

  return result;
}

void Renderer::renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform, GLenum mode) {
  glBindVertexArray(mesh->vao());

  if (mesh->hasIndices()) {
    glDrawElements(mode, mesh->indexCount(), GL_UNSIGNED_SHORT, 0);
  } else {
    glDrawArrays(mode, 0, mesh->indexCount());
  }
}

void Renderer::setupMaterialBindings(RenderOperation *rop) {
//  material->setProjection(state.projectionMatrix);
//  material->setView(state.viewMatrix);

  if (rop->material->hasTransformBlock()) {
    UBOStruct::TransformStruct transformStruct;
    transformStruct.transform = rop->modelMatrix;
    transformStruct.normalMatrix = glm::inverseTranspose(rop->modelMatrix);
    rop->material->setTransformBlock(transformStruct);
  }

  _uboManager->processMeterialBindings(rop);
}

void Renderer::renderScene(Scene &scene) {
  for (auto &camera : *scene.cameras()) {
    _renderCamera(scene, std::const_pointer_cast<Camera>(camera.second));
  }
}

void Renderer::_clearQueues() {
  _ropCounter = 0;
  for (auto &queue : _queues) {
    queue.clear();
  }
}

void Renderer::_renderCamera(Scene &scene, CameraPtr camera) {
  state.projectionMatrix = camera->projectionMatrix();
  state.viewMatrix = camera->viewMatrix();

  _clearQueues();

  auto visibleObjects = scene.visibleObjects(camera);
  for (auto &object : *visibleObjects) {
    object->render(*this);
  }

  _debugDraw->render(*this);

  _prepareQueues(scene, camera);

  auto lights = scene.visibleLights(camera);
  _lightGrid->appendLights(lights, camera);
  _lightGrid->upload();
  _lightGrid->bindBufferTextures();

  _uboManager->setCamera(camera);
  _uboManager->updateLights(lights);
  _uboManager->upload();

  _processRenderPipeline();
}

void Renderer::_prepareQueues(Scene &scene, CameraPtr camera) {
  _uboManager->swap();

  for (auto &queue : _queues) {
    for (auto &rop : queue) {
      setupMaterialBindings(&rop);
    }
  }
}

void Renderer::_processRenderPipeline() {
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

void Renderer::postUpdate(float dt) {
  _lightGrid->update(_window->width(), _window->height());
}
