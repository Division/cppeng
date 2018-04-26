//
// Created by Sidorenko Nikita on 3/24/18.
//

#include "Window.h"
#include "Logging.h"
#include "EngineGL.h"
#include "Input.h"

//#ifdef __EMSCRIPTEN__
//#include <emscripten.h>
//#include <emscripten/html5.h>
//
//EM_BOOL onWebResize(int eventType, const EmscriptenUiEvent* event, void* userData) {
//  double w, h;
//  emscripten_get_element_css_size(nullptr, &w, &h);
//  ENGLog("Window resize %fx%f", w, h);
//}
//#endif


void Window::quit() {
#ifdef __EMSCRIPTEN__
  emscripten_log(EM_LOG_NO_PATHS, "Print a log message: int: %d, string: %s.", 42, "hello");
  emscripten_cancel_main_loop();
#else
  SDL_DestroyWindow(_window);
  SDL_Quit();
#endif
}

void Window::initOpenGLWindow(int width, int height) {

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
#else
//  emscripten_set_resize_callback(NULL, this, true, onWebResize);
#endif

  _window = SDL_CreateWindow("engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
  if (!_window) {
    printf("Error creating window\n");
  }

  SDL_SetWindowResizable(_window, SDL_FALSE);

  _context = SDL_GL_CreateContext(_window);
  SDL_GL_SetSwapInterval(1);

  _updateSize();
  ENGLog("Window created with resolution %ix%i", _width, _height);
  ENGLog("GL VERSION: %s", glGetString(GL_VERSION));
}

void Window::_updateSize () {
#ifdef __EMSCRIPTEN__
//  int w, h;
//  emscripten_get_element_css_size(nullptr, &w, &h);
//  ENGLog("Window resize %fx%f", w, h);
#endif

  SDL_GL_GetDrawableSize(_window, &_width, &_height);
}

void Window::swapBuffers() {
  SDL_GL_SwapWindow(_window);
}

void Window::processEvents() {
  SDL_Event e;

  _input->_prepareForUpdate();

  auto prevSize = size();

  int hasPendingEvents;
  do {
    hasPendingEvents = SDL_PollEvent(&e);

    switch (e.type) {
      case SDL_QUIT:
        _quitTriggered = true;
        break;

      case SDL_WINDOWEVENT:
        _updateSize();
        _processSDLWindowEvent(e.window);
        break;
    }

    _input->updateWithSDLEvent(e);
  } while (hasPendingEvents);


  _sizeChangedLastFrame = prevSize != size();
}

void Window::_processSDLWindowEvent(SDL_WindowEvent &e) {
  if (e.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.event == SDL_WINDOWEVENT_RESIZED) {
    _updateSize();
  }
}

