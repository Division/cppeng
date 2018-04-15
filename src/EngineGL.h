//
// Created by Sidorenko Nikita on 3/22/18.
//

#ifndef CPPWRAPPER_ENGINEGL_JS_H
#define CPPWRAPPER_ENGINEGL_JS_H

#include "SDL_opengl.h"

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
  };

}

#endif //CPPWRAPPER_ENGINEGL_JS_H
