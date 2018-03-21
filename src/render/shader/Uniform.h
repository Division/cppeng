//
// Created by Sidorenko Nikita on 3/19/18.
//

#ifndef CPPWRAPPER_UNIFORM_H
#define CPPWRAPPER_UNIFORM_H

#include "SDL_opengl.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>

using namespace glm;

enum class UniformType : int {
  None = 0,
  ProjectionMatrix,
  ModelViewMatrix,
  ViewMatrix,
  Count
};

extern const std::map<UniformType, std::string> UNIFORM_NAMES;

class Uniform {
public:
  Uniform () : _location(0) {}
  Uniform(GLint location, UniformType type) :
      _location(location),
      _type(type) {}

  void setMatrix(const mat4 &matrix) {
    glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

protected:
  GLint _location;
  UniformType _type = UniformType::None;
};


#endif //CPPWRAPPER_UNIFORM_H
