//
// Created by Sidorenko Nikita on 1/21/18.
//

#include "Shader.h"
#include <system/Logging.h>
#include <engine/EngineMain.h>
#include "EngineGL.h"

const std::map<ShaderAttrib, std::string> SHADER_ATTRIB_NAMES = {
    { ShaderAttrib::Position, "aPosition" },
    { ShaderAttrib::TexCoord0, "aTexCoord0" }
};

const std::string VERTEX_STR = "vertex";
const std::string FRAGMENT_STR = "fragment";

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
  ENGLog("VERTEX SHADER SRC: %s", vertexSource.c_str());
  ENGLog("FRAGMENT SHADER SRC: %s", fragmentSource.c_str());
  GLuint program = this->_compileShader(vertexSource, fragmentSource);
  if (program) {
    this->_program = program;
  }
}

// Returns shader program or 0
GLuint Shader::_compileShader(const std::string &vertexSource, const std::string &fragmentSource) {
  GLuint vertexShader = this->_loadShader(vertexSource, GL_VERTEX_SHADER);
  GLuint fragmentShader = this->_loadShader(fragmentSource, GL_FRAGMENT_SHADER);

  if (!vertexShader || !fragmentShader) {
    return 0;
  }

  GLuint program = glCreateProgram();

  this->_setupAttribs(program);
  engine::checkGLError();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint linkResult;
  glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

  if (!linkResult) {
    GLsizei length = 0;
    GLchar message[1024];
    glGetProgramInfoLog(program, 1024, &length, message);
    ENGLog("Error linking shader: %s", message);
    return 0;
  }

  return program;
}

GLuint Shader::_loadShader(const std::string &source, GLenum type) {
  GLuint shader = glCreateShader(type);
  GLint len = source.length();
  const GLchar *sourcePtr = source.c_str();
  glShaderSource(shader, 1, &sourcePtr, &len);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {
    GLsizei length = 0;
    GLchar message[1024];
    glGetShaderInfoLog(shader, 1024, &length, message);
    const std::string *shaderType = type == GL_VERTEX_SHADER ? &VERTEX_STR : &FRAGMENT_STR;
    ENGLog("Error loading %s shader: %s", shaderType->c_str(), message);
    shader = 0;
  }

  return shader;
}

void Shader::_setupAttribs(GLuint program) {
  for (auto &keyValue : SHADER_ATTRIB_NAMES) {
    glBindAttribLocation(program, (GLuint)keyValue.first, keyValue.second.c_str());
  }
}

void Shader::notifyDirty(ICleanable *uniform) {

}

void Shader::bind() {
  glUseProgram(_program);
}

Uniform *Shader::addUniform(const UniformType type) {
  auto &uniformName = UNIFORM_NAMES.at(type);
  GLint location = glGetUniformLocation(_program, uniformName.c_str());
  if (location == -1) {
    ENGLog("Can't find uniform %s", uniformName.c_str());
  }
  _uniforms[(int)type] = std::move(std::unique_ptr<Uniform>(new Uniform(location, type)));
  return this->getUniform(type);
}

Uniform *Shader::getUniform (const UniformType type) {
  return _uniforms[(int)type].get();
}

//------------------------------------------------------------------------
// ShaderCapsSet
//------------------------------------------------------------------------

bool ShaderCapsSet::hasCap(const ShaderCaps cap) const {
  return _caps.find((int)cap) != _caps.end();
}

void ShaderCapsSet::addCap(const ShaderCaps cap) {
  _caps.insert((int)cap);
  _maskDirty = true;
}

void ShaderCapsSet::removeCap(const ShaderCaps cap) {
  _caps.erase((int)cap);
  _maskDirty = true;
}

ShaderCapsSet::Bitmask ShaderCapsSet::getBitmask() const {
  if (_maskDirty) {
    _bitmask = _calculateBitMask();
    _maskDirty = false;
  }
  return _bitmask;
}

ShaderCapsSet::Bitmask ShaderCapsSet::_calculateBitMask() const {
  Bitmask result = 0;

  for (auto cap : _caps) {
    result |= (Bitmask)cap;
  }

  return result;
}
