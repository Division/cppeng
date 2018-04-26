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
  auto engine = static_cast<Engine*>(arg);

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
  if (engine->_window->sizeChangedLastFrame()) {

  }
  engine->update(dt);
  engine->_window->swapBuffers();
  engine->_window->processEvents();

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

void Engine::update(double dt) {
  glViewport(0, 0, _window->width(), _window->height());
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);

  _game->update((float)dt);
  _renderer->postUpdate((float) dt);

  engine::checkGLError();
}

void Engine::init() {
  engine::GLCaps::init(); // Setup OpenGL caps
  _renderer = new Renderer(_window);
  _renderer->setupShaders();
  _game->init(this);
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