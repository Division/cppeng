#include "Engine.h"
#include "EngineGL.h"
#include <iostream>
#include "system/Window.h"
#include "system/Input.h"
#include "Resources.h"
#include <system/Logging.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine.h>

#include "render/mesh/Mesh.h"
#include "render/shader/Shader.h"
#include "render/renderer/Renderer.h"
#include "loader/TextureLoader.h"
#include "render/texture/Texture.h"
#include "render/shader/ShaderGenerator.h"
#include "EngMath.h"
#include "render/shader/ShaderCaps.h"
#include "scene/Scene.h"
#include "render/renderer/SceneRenderer.h"
#include "EngineMain.h"
#include "utils/Performance.h"

using namespace glm;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Engine::Engine() {
  _window = std::make_shared<Window>();
  _generator = std::make_shared<ShaderGenerator>();
  _input = std::make_shared<Input>(_window);
}

Engine::~Engine() {

}

void Engine::quit() {
  _window->quit();
  this->_shouldQuit = true;
}

bool shouldPrint = false;

void mainLoop(void *arg) {
  engine::Performance::startTimer(engine::Performance::Entry::Frame);

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
  engine->_window->processEvents();
  engine->update(dt);

  engine::Performance::startTimer(engine::Performance::Entry::SwapBuffers);
  engine->_window->swapBuffers();
  engine::Performance::stopTimer(engine::Performance::Entry::SwapBuffers);

  if (engine->_window->quitTriggered()) {
    engine->quit();
  }

  engine::Performance::stopTimer(engine::Performance::Entry::Frame);
}

void Engine::printStatus() {
//  shouldPrint = true;
//  printf("STATUS PRINTED\n");
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
    emscripten_set_main_loop_arg(mainLoop, this, -1, 1);
}
#endif

void Engine::setup(std::weak_ptr<IGame> game) {
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

  _input->_update();
  _game.lock()->update((float)dt);

  engine::checkGLError();
}

void Engine::init() {
  engine::GLCaps::init(); // Setup OpenGL caps
  _generator->setupTemplates();
  _sceneRenderer = std::make_shared<SceneRenderer>();

  engine::Performance::initialize();

  _game.lock()->init(getEngine());
  engine::checkGLError();
}

void Engine::renderScene(std::shared_ptr<Scene> scene) {
  _sceneRenderer->renderScene(scene);
}

std::shared_ptr<DebugDraw> Engine::debugDraw() const {
  return _sceneRenderer->debugDraw();
}

ShaderPtr Engine::getShaderWithCaps (std::shared_ptr<ShaderCapsSet> caps) const {
  return _generator->getShaderWithCaps(caps);
}

void Engine::projectorTexture(const std::shared_ptr<Texture> texture) {
  _sceneRenderer->projectorTexture(texture);
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