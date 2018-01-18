#include "engine/Engine.h"
#include <iostream>
#include "SDL.h"
#include "SDL_opengl.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Engine::Engine() {

}

SDL_Window* window;
SDL_Renderer* renderer;

void Engine::quit() {
  printf("QUIT\n");

#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
#else
  SDL_DestroyWindow(window);
  SDL_Quit();
#endif

  this->_shouldQuit = true;
}

bool shouldPrint = false;

void mainLoop(void *arg) {
    if (shouldPrint) {
      std::cout << "asdasd" << std::endl;
      glClearColor(1, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    Engine *engine = static_cast<Engine*>(arg);
    SDL_Event e;

    SDL_PollEvent(&e);

    if (e.type == SDL_QUIT) {
      engine->quit();
    }
    if (e.type == SDL_KEYDOWN) {
      engine->quit();
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
      engine->quit();
    }
}

void Engine::printStatus() {
  shouldPrint = true;
  printf("STATUS PRINTED\n");
  SDL_Log("STATUS PRINTED\n");
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::startSDLLoop() {
    while (!this->_shouldQuit) {
        mainLoop(this);
    }
}

#ifdef __EMSCRIPTEN__
void Engine::startEmscriptenLoop() {
    printf("Starting EMSCRIPTEN loop\n");
    emscripten_set_main_loop_arg(mainLoop, this, -1, 0);
}
#endif

void Engine::setupSDL() {
    printf("Engine start\n");
    SDL_Log("SDL START\n");

    SDL_Window *window;
    SDL_GLContext context;

    printf("BEFORE INIT\n");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error init sdl\n");
    }
  printf("AFTER INIT\n");

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("sdlglshader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Error creating window\n");
    }

    context = SDL_GL_CreateContext(window);

    glClearColor(1, 0, 1, 1);
    glViewport(0, 0, 640, 480);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);

    printf("Engine window created");

#ifdef __EMSCRIPTEN__
    this->startEmscriptenLoop();
#else
    this->startSDLLoop();
#endif

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