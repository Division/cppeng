#include "engine/Engine.h"
#include "EngineGL.h"
#include <iostream>
#include "system/Window.h"
#include "system/Input.h"
#include "Resources.h"
#include <system/Logging.h>
#include <glm/gtc/matrix_transform.hpp>
#include "render/mesh/Mesh.h"
#include "render/shader/Shader.h"
#include "render/renderer/Renderer.h"
#include "loader/TextureLoader.h"
#include "render/texture/Texture.h"
#include "EngMath.h"

#include "scene/Scene.h"

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

void Engine::startEngineLoop() {
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

void Engine::setup(IGame *game) {
  _window->initOpenGLWindow(640, 480);
  _game = game;

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  this->init();

#ifdef __EMSCRIPTEN__
  this->startEmscriptenLoop();
#else
  this->startEngineLoop();
#endif

}

Mesh *mesh;
Shader *shader = nullptr;
GLuint vbo;
float ang = 0;
TexturePtr tex;

void Engine::update(double dt) {

  _game->update(dt);

  if (!shader) {
    return;
  }

  if (_input->keyDown(Key::Space)) {
    ShaderCapsSetPtr caps(new ShaderCapsSet());
    ENGLog("GENERATED: %s", _renderer->generator()->generateShaderSource(caps).c_str());
  }

  glClearColor(1, 0, 0, 1);
  glViewport(0, 0, _window->width(), _window->height());
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  mat4x4 projection =  glm::perspective(glm::radians(45.f), _window->aspect(), 0.1f, 1000.0f);
  mat4 modelview(1.0f);
  modelview = glm::translate(modelview, vec3(0, 0, -10));
  modelview = glm::rotate(modelview, ang, vec3(0, 0, 1));
  ang += 3 * dt;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex->id());

  shader->bind();
  shader->getUniform(UniformType::ProjectionMatrix)->setMatrix(projection);
  shader->getUniform(UniformType::ModelViewMatrix)->setMatrix(modelview);
  shader->getUniform(UniformType::Texture0)->setTexture(0);

  glBindVertexArray(mesh->vao());
  glDrawElements(GL_TRIANGLES, mesh->indexCount(), GL_UNSIGNED_SHORT, 0);

  engine::checkGLError();
}

void Engine::init() {
  _renderer->setupShaders();
  _game->init(this);

  mesh = new Mesh();

  GLushort indices[] = {0, 1, 2, 0, 2, 3};
  float vertices[] = {    -1.0f, -1.0f, 0.0f,
                          1.0f, -1.0f, 0.0f,
                          1.0f, 1.0f, 0.0f,
                          -1.0f,  1.0f, 0.0f };

  float texcoords[] = {   0, 0,
                          1.0f, 0,
                          1.0f, 1.0f,
                          0,  1.0f };

  mesh->setVertices(vertices, 4);
  mesh->setIndices(indices, 6);
  mesh->setTexCoord0(texcoords, 4);
  mesh->createBuffer();

  tex = loader::loadTexture("resources/platform.png");

  ShaderCapsSetPtr caps(new ShaderCapsSet());

  shader = _renderer->getShaderWithCaps(caps).get();
  shader->addUniform(UniformType::ProjectionMatrix);
  shader->addUniform(UniformType::ModelViewMatrix);
  shader->addUniform(UniformType::Texture0);
  engine::checkGLError();
}

void Engine::renderScene(Scene &scene) {
  _renderer->renderScene(scene);
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