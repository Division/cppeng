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
    case GL_STACK_OVERFLOW:    errorStr = "Stack overflow"; break;
    case GL_STACK_UNDERFLOW:   errorStr = "Stack underflow"; break;
    case GL_OUT_OF_MEMORY:     errorStr = "Out of memory"; break;
    default:                   errorStr = "Unknown error"; break;
  }

  ENGLog("GL Error: %s", errorStr.c_str());
}


GLint engine::GLCaps::_maxUBOSize = 0;
GLint engine::GLCaps::_uboOffsetAlignment = 0;
void engine::GLCaps::init() {
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &_maxUBOSize);
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &_uboOffsetAlignment);

  ENGLog("OpenGL Caps:");
  ENGLog("    GL_MAX_UNIFORM_BLOCK_SIZE: %i", _maxUBOSize);
  ENGLog("    GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT: %i", _uboOffsetAlignment);
  ENGLog("");
}
