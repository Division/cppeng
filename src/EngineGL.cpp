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