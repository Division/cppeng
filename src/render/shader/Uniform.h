//
// Created by Sidorenko Nikita on 3/19/18.
//

#ifndef CPPWRAPPER_UNIFORM_H
#define CPPWRAPPER_UNIFORM_H

#include "EngineGL.h"
#include "EngMath.h"
#include <string>
#include <map>
#include "render/texture/Texture.h"

using namespace glm;

enum class UniformName : int {
  None = 0,
  ProjectionMatrix,
  ModelViewMatrix,
  ViewMatrix,
  Texture0,
  Texture1,
  NormalMap,
  Count
};

extern const std::map<UniformName, std::string> UNIFORM_NAMES;

class Uniform {
public:
  Uniform () : _location(0) {}
  Uniform(GLint location, UniformName type) :
      _location(location),
      _type(type) {}

  void setMatrix(const mat4 &matrix) {
    glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void setTexture(int index) {
    glUniform1i(_location, index);
  }

protected:
  GLint _location;
  UniformName _type = UniformName::None;
};


#endif //CPPWRAPPER_UNIFORM_H
