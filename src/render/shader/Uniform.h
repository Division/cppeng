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
  Color,
  ProjectionMatrix,
  ModelViewMatrix,
  ViewMatrix,
  NormalMatrix,
  Texture0,
  Texture1,
  NormalMap,
  Count
};

enum class UniformBlockName : int {
  None = 0,
  Transform,
  Light,
  Count
};

extern const std::map<UniformName, std::string> UNIFORM_NAMES;
extern const std::map<UniformBlockName, std::string> UNIFORM_BLOCK_NAMES;

class Uniform {
public:
  Uniform () : _location(0) {}
  Uniform(GLint location, UniformName type) :
      _location(location),
      _type(type) {}

  void setMat4(const mat4 &matrix) {
    glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void setMat3(const mat3 &matrix) {
    glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void setVec4(const vec4 &vec) {
    glUniform4fv(_location, 1, glm::value_ptr(vec));
  }

  void setVec3(const vec3 &vec) {
    glUniform3fv(_location, 1, glm::value_ptr(vec));
  }

  void setVec2(const vec2 &vec) {
    glUniform2fv(_location, 1, glm::value_ptr(vec));
  }

  void setInt(int index) {
    glUniform1i(_location, index);
  }

protected:
  GLint _location;
  UniformName _type = UniformName::None;
};

class UniformBlock {
public:
  UniformBlock(UniformBlockName name, GLuint index, int size) : _name(name), _index(index), _size(size) {};

  GLint bindingPoint() { return (GLint)_name; }
protected:
  UniformBlockName _name = UniformBlockName::None;
  GLuint _index = 0;
  int _size = 0;
};


#endif //CPPWRAPPER_UNIFORM_H
