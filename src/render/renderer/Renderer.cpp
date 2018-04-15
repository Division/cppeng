//
// Created by Sidorenko Nikita on 2/18/18.
//

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

using namespace glm;

Renderer::Renderer() {
  _uboManager = std::make_unique<UBOManager>();
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

void Renderer::renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform) {
//  setupMaterialBindings(material, transform);

  glBindVertexArray(mesh->vao());
  glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_SHORT, 0, 1);
}

void Renderer::setupMaterialBindings(MaterialPtr &material, const mat4 &transform) {
  mat4 modelView = state.viewMatrix * transform;
  material->setProjection(state.projectionMatrix);
  material->setView(state.viewMatrix);

  if (material->hasTransformBlock()) {
    TransformStruct transformStruct;
    transformStruct.transform = transform;
    material->setTransformBlock(transformStruct);
  }

  _uboManager->processMeterialBindings(material);
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

  _prepareQueues();
  _processRenderPipeline();
}

void Renderer::_prepareQueues() {
  _uboManager->swap();

  // Opaque
  auto &opaqueQueue = _queues[(int)RenderQueue::Opaque];
  for (auto &rop : opaqueQueue) {
    setupMaterialBindings(rop.material, rop.modelMatrix);
  }

  _uboManager->upload();
}

void Renderer::_processRenderPipeline() {
  // Opaque
  auto &opaqueQueue = _queues[(int)RenderQueue::Opaque];
  for (auto &rop : opaqueQueue) {
    _uboManager->setupForRender(rop.material);
    renderMesh(rop.mesh, rop.material, rop.modelMatrix);
  }
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
