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

using namespace glm;

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

void Renderer::renderMesh(Mesh &mesh, Material &material, const mat4 &transform) {
  mat4x4 projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 1000.0f);

  material.setProjection(projection);
  material.setModelView(transform);

  material.shader()->bind();
  material.uploadBindings();

  glBindVertexArray(mesh.vao());
  glDrawElements(GL_TRIANGLES, mesh.indexCount(), GL_UNSIGNED_SHORT, 0);
}

void Renderer::renderScene(Scene &scene) {
  auto visibleObjects = scene.visibleObjects();
  for (auto object : visibleObjects) {
    object->render(*this);
  }

  _processRenderPipeline();
}

void Renderer::_processRenderPipeline() {
  // Handling passes, render queues etc
}

