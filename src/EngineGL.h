//
// Created by Sidorenko Nikita on 3/22/18.
//

#ifndef CPPWRAPPER_ENGINEGL_JS_H
#define CPPWRAPPER_ENGINEGL_JS_H

#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#define ENGINE_USE_BUFFER_TEXTURE true
#else
#define ENGINE_USE_BUFFER_TEXTURE false
#endif
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

namespace engine {

  void checkGLError();

  class GLCaps {
  public:
    static void init();

    static GLint maxUBOSize() { return _maxUBOSize; }
    static GLint uboOffsetAlignment() { return _uboOffsetAlignment; }
    static GLint minMapBufferAlighment() { return _minMapBufferAlighment; }

  private:
    // UBO
    static GLint _maxUBOSize;
    static GLint _uboOffsetAlignment;
    static GLint _maxUBOBindings;

    // Buffer
    static GLint _minMapBufferAlighment;

    // Texture
    static GLint _maxTextureSize;
  };

}

#endif //CPPWRAPPER_ENGINEGL_JS_H
