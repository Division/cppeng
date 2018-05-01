//
// Created by Sidorenko Nikita on 3/22/18.
//

#ifndef CPPWRAPPER_ENGINEGL_JS_H
#define CPPWRAPPER_ENGINEGL_JS_H

#ifndef __EMSCRIPTEN__
#include "SDL_opengl.h"
#else
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

namespace engine {

  void checkGLError();

  class GLCaps {
  public:
    static void init();

    static GLint maxUBOSize() { return _maxUBOSize; }
    static GLint uboOffsetAlignment() { return _uboOffsetAlignment; }

  private:
    // UBO
    static GLint _maxUBOSize;
    static GLint _uboOffsetAlignment;
    static GLint _maxUBOBindings;

    // Texture
    static GLint _maxTextureSize;
  };

}

#endif //CPPWRAPPER_ENGINEGL_JS_H
