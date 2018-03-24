#include "engine/Engine.h"
#include <iostream>
#include "SDL.h"
#include "SDL_opengl.h"
#include "Resources.h"
#include <Logging.h>
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
}

SDL_Window* window;
SDL_GLContext context;
SDL_Renderer* renderer;

void Engine::quit() {
#ifdef __EMSCRIPTEN__
  emscripten_log(EM_LOG_NO_PATHS, "Print a log message: int: %d, string: %s.", 42, "hello");
  emscripten_cancel_main_loop();
#else
  SDL_DestroyWindow(window);
  SDL_Quit();
#endif

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
  SDL_GL_SwapWindow(window);

//  ENGLog("TIME: %f", engine->_currentTime);

  SDL_Event e;

  SDL_PollEvent(&e);

  if (e.type == SDL_QUIT) {
    engine->quit();
  }
//  if (e.type == SDL_KEYDOWN) {
//    engine->quit();
//  }
  if (e.type == SDL_MOUSEBUTTONDOWN) {
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
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      ENGLog("Error init sdl\n");
  };

// breaks webgl build
#ifndef __EMSCRIPTEN__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif

  window = SDL_CreateWindow("engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
  if (!window) {
      printf("Error creating window\n");
  }

  ENGLog("SDL ERR: %s", SDL_GetError());
  context = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(0);

  GLint min = 0;
  GLint maj = 0;
  glGetIntegerv(GL_MAJOR_VERSION, &maj);
  glGetIntegerv(GL_MINOR_VERSION, &min);

  ENGLog("GL VERSION %i.%i / %s", maj, min, glGetString(GL_VERSION));

  ENGLog("Engine window created. Starting mainloop.");

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

  this->checkGLError();
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

//  Resources::loadShader("resources/shaders/white.shader");

  ShaderCapsSetPtr caps(new ShaderCapsSet());

  shader = _renderer->getShaderWithCaps(caps).get();
  shader->addUniform(UniformType::ProjectionMatrix);
  shader->addUniform(UniformType::ModelViewMatrix);
  this->checkGLError();
}

void Engine::checkGLError() {

  const GLenum err = glGetError();
  if (GL_NO_ERROR == err) {
    return;
  }

  std::string errorStr;

  switch (err) {
    case GL_INVALID_ENUM:      errorStr = "Invalid enum"; break;
    case GL_INVALID_VALUE:     errorStr = "Invalid value"; break;
    case GL_INVALID_OPERATION: errorStr = "Invalid operation"; break;
    case GL_STACK_OVERFLOW:    errorStr = "Stack overflow"; break;
    case GL_STACK_UNDERFLOW:   errorStr = "Stack underflow"; break;
    case GL_OUT_OF_MEMORY:     errorStr = "Out of memory"; break;
    default:                   errorStr = "Unknown error"; break;
  }

  ENGLog("GL Error: %s", errorStr.c_str());
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