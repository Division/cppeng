//
// Created by Sidorenko Nikita on 3/24/18.
//

#include "Window.h"
#include "Logging.h"
#include "EngineGL.h"
#include "Input.h"

void Window::quit() {
#ifdef __EMSCRIPTEN__
  emscripten_log(EM_LOG_NO_PATHS, "Print a log message: int: %d, string: %s.", 42, "hello");
  emscripten_cancel_main_loop();
#else
  SDL_DestroyWindow(_window);
  SDL_Quit();
#endif
}

void Window::initOpenGLWindow() {
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

  _window = SDL_CreateWindow("engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
  if (!_window) {
    printf("Error creating window\n");
  }

  ENGLog("SDL ERR: %s", SDL_GetError());
  _context = SDL_GL_CreateContext(_window);
  SDL_GL_SetSwapInterval(0);

  ENGLog("GL VERSION: %s", glGetString(GL_VERSION));
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(_window);
}

void Window::processEvents() {
  SDL_Event e;
  SDL_PollEvent(&e);

  if (e.type == SDL_QUIT) {
    _quitTriggered = true;
  }

  _input->updateWithSDLEvent(e);
}
