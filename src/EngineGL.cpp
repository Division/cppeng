//
// Created by Sidorenko Nikita on 3/24/18.
//

#import "EngineGL.h"
#import <string>
#import "system/Logging.h"

void engine::checkGLError() {

  auto err = glGetError();
  if (GL_NO_ERROR == err) {
    return;
  }

  std::string errorStr;

  switch (err) {
    case GL_INVALID_ENUM:      errorStr = "Invalid enum"; break;
    case GL_INVALID_VALUE:     errorStr = "Invalid value"; break;
    case GL_INVALID_OPERATION: errorStr = "Invalid operation"; break;
    case GL_OUT_OF_MEMORY:     errorStr = "Out of memory"; break;
    default:                   errorStr = "Unknown error"; break;
  }

  ENGLog("GL Error: %s", errorStr.c_str());
}


GLint engine::GLCaps::_maxUBOSize = 0;
GLint engine::GLCaps::_uboOffsetAlignment = 0;
GLint engine::GLCaps::_maxTextureSize = 0;
GLint engine::GLCaps::_maxUBOBindings = 0;

void engine::GLCaps::init() {
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &_maxUBOSize);
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &_uboOffsetAlignment);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
  glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &_maxUBOBindings);

  ENGLog("OpenGL Caps:");
  ENGLog("    GL_MAX_UNIFORM_BLOCK_SIZE: %i", _maxUBOSize);
  ENGLog("    GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT: %i", _uboOffsetAlignment);
  ENGLog("    GL_MAX_UNIFORM_BUFFER_BINDINGS: %i", _maxUBOBindings);
  ENGLog("    GL_MAX_TEXTURE_SIZE: %i", _maxTextureSize);
  ENGLog("");
}
