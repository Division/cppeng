//
// Created by Sidorenko Nikita on 3/24/18.
//

#include "Window.h"
#include "Logging.h"
#include "EngineGL.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif


void Window::quit() {
#ifdef __EMSCRIPTEN__
  emscripten_log(EM_LOG_NO_PATHS, "Print a log message: int: %d, string: %s.", 42, "hello");
  emscripten_cancel_main_loop();
#else
  glfwDestroyWindow(_window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
#endif
}

void Window::initOpenGLWindow(int width, int height) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_SRGB_CAPABLE, GL_FALSE);
#ifndef __EMSCRIPTEN__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  _window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
  glfwMakeContextCurrent(_window);

#ifndef __EMSCRIPTEN__
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
#endif
//  glfwSetWindowSizeCallback(_window, window_size_callback);

  glfwSwapInterval(0);
  _updateSize();
  ENGLog("Window created with resolution %ix%i", _width, _height);
  ENGLog("GL VERSION: %s", glGetString(GL_VERSION));
}

void Window::_updateSize () {
  glfwGetFramebufferSize(_window, &_width, &_height);
}

void Window::swapBuffers() {
  glfwSwapBuffers(_window);
}

void Window::processEvents() {
  glfwPollEvents();
  _updateSize();
}
