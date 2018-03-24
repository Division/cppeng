#include "engine/Engine.h"
#include <iostream>
#include "EngineGL.h"
#include "system/Window.h"
#include "system/Input.h"
#include "Resources.h"
#include <system/Logging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render/mesh/Mesh.h"
#include <loader/ShaderLoader.h>
#include "render/shader/Shader.h"
#include "render/renderer/Renderer.h"

using namespace glm;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Engine::Engine() {
  _renderer = new Renderer();
  _input = new Input();
  _window = new Window(_input);
}

Engine::~Engine() {
  delete _renderer;
  delete _window;
  delete _input;
}

void Engine::quit() {
  _window->quit();
  this->_shouldQuit = true;
}

bool shouldPrint = false;

void mainLoop(void *arg) {
  Engine *engine = static_cast<Engine*>(arg);

  // Time
  auto now = std::chrono::system_clock::now().time_since_epoch();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);

  auto currentTime = ms.count() / 1000.0;

  if (!engine->_startTime) {
    engine->_startTime = currentTime;
    engine->_currentTime = currentTime;
  }

  double dt = currentTime - engine->_currentTime;

  engine->_currentTime = currentTime;

  // Update
  engine->update(dt);
  engine->_window->swapBuffers();
  engine->_window->processEvents();

//  ENGLog("TIME: %f", engine->_currentTime);

  if (engine->_window->quitTriggered()) {
    engine->quit();
  }

}

void Engine::printStatus() {
//  shouldPrint = true;
//  printf("STATUS PRINTED\n");
//  SDL_Log("STATUS PRINTED\n");
//  glClearColor(0, 0, 0, 1);
//  glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::startSDLLoop() {
  while (!this->_shouldQuit) {
    mainLoop(this);
  }
}

#ifdef __EMSCRIPTEN__
void Engine::startEmscriptenLoop() {
    ENGLog("Starting EMSCRIPTEN loop");
    emscripten_set_main_loop_arg(mainLoop, this, -1, 0);
}
#endif

void Engine::setupSDL() {
  _window->initOpenGLWindow();

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  this->init();

#ifdef __EMSCRIPTEN__
  this->startEmscriptenLoop();
#else
  this->startSDLLoop();
#endif

}

Mesh *mesh;
Shader *shader = nullptr;
GLuint vbo;
float ang = 0;

void Engine::update(double dt) {
  if (!shader) {
    return;
  }

  if (_input->keyDown(Key::Space)) {
    ENGLog("SPACE");
  }

  glClearColor(1, 0, 0, 1);
  glViewport(0, 0, 640, 480);
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  mat4x4 projection =  glm::perspective(glm::radians(45.f), 640.0f / 480.0f, 0.1f, 1000.0f);
  mat4 modelview(1.0f);
  modelview = glm::translate(modelview, vec3(0, 0, -10));
  modelview = glm::rotate(modelview, ang, vec3(0, 0, 1));
  ang += 3 * dt;

  shader->bind();
  shader->getUniform(UniformType::ProjectionMatrix)->setMatrix(projection);
  shader->getUniform(UniformType::ModelViewMatrix)->setMatrix(modelview);

  glBindVertexArray(mesh->vao());
  glDrawArrays(GL_TRIANGLES, 0, 3);

  engine::checkGLError();
}

void Engine::init() {
  _renderer->setupShaders();

  mesh = new Mesh();

  unsigned short indices[] = {0, 1, 2};
  float vertices[] = {    -1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          0.0f,  1.0f, 0.0f };

  mesh->setVertices(vertices, 3);
  mesh->setIndices(indices, 3);
  mesh->createBuffer();

  ShaderCapsSetPtr caps(new ShaderCapsSet());

  shader = _renderer->getShaderWithCaps(caps).get();
  shader->addUniform(UniformType::ProjectionMatrix);
  shader->addUniform(UniformType::ModelViewMatrix);
  engine::checkGLError();
}

// JS Bindings
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(engine_main) {
    emscripten::class_<Engine>("Engine")
        .function("printStatus", &Engine::printStatus)
    ;
}
#endif