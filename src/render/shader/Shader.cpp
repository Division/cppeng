//
// Created by Sidorenko Nikita on 1/21/18.
//

#include "Shader.h"
#include <system/Logging.h>
#include <memory>

struct ShaderConfig {
  std::vector<UniformName > uniforms;
  std::vector<UniformBlockName> uniformBlocks;
};

const std::vector<UniformBlockName> DEFAULT_UBO = { UniformBlockName::Transform, UniformBlockName::Camera };

const std::map<ShaderCaps, ShaderConfig> UNIFORMS_PER_CAP = {
    { ShaderCaps::ObjectData, { {}, DEFAULT_UBO } },
    { ShaderCaps::Color, { { UniformName::Color }, DEFAULT_UBO } },
    { ShaderCaps::Lighting, {
      { UniformName::ShadowMap, UniformName::LightGrid, UniformName::LightIndices, UniformName::ProjectorTexture},
      { UniformBlockName::Transform, UniformBlockName::Camera, UniformBlockName::Light, UniformBlockName::Projector }
    } },
    { ShaderCaps::NormalMap, { { UniformName::NormalMap }, DEFAULT_UBO } },
    { ShaderCaps::Texture0, { { UniformName::Texture0 }, DEFAULT_UBO } },
    { ShaderCaps::VertexColor, { {}, DEFAULT_UBO } },
    { ShaderCaps::SpecularMap, { { UniformName::SpecularMap }, {} } },
    { ShaderCaps::TerrainLayer0, { { UniformName::TerrainDiffuse0, UniformName::TerrainNormal0 }, { DEFAULT_UBO } } },
    { ShaderCaps::TerrainLayer1, { { UniformName::TerrainDiffuse1, UniformName::TerrainNormal1, UniformName::TerrainSplatmap }, {} } },
    { ShaderCaps::TerrainLayer2, { { UniformName::TerrainDiffuse2, UniformName::TerrainNormal2, UniformName::TerrainSplatmap }, {} } },
    { ShaderCaps::Skinning, { {}, { UniformBlockName::SkinningMatrices, UniformBlockName::Transform, UniformBlockName::Camera } } }
};

const std::map<ShaderAttrib, std::string> SHADER_ATTRIB_NAMES = {
  { ShaderAttrib::Position, "aPosition" },
  { ShaderAttrib::Normal, "aNormal" },
  { ShaderAttrib::Tangent, "aTangent" },
  { ShaderAttrib::Bitangent, "aBitangent" },
  { ShaderAttrib::TexCoord0, "aTexCoord0" },
  { ShaderAttrib::VertexColor, "aVertexColor" },
  { ShaderAttrib::JointWeights, "aJointWeights" },
  { ShaderAttrib::JointIndices, "aJointIndices" }
};

const std::string VERTEX_STR = "vertex";
const std::string FRAGMENT_STR = "fragment";

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
//  ENGLog("VERTEX SHADER SRC: %s", vertexSource.c_str());
//  ENGLog("FRAGMENT SHADER SRC: %s", fragmentSource.c_str());
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

void Shader::unbind() {
  glUseProgram(0);
}

//------------------------------------------------------------------------
// Uniform
//------------------------------------------------------------------------

UniformBlock *Shader::addUniformBlock(const UniformBlockName type) {
  auto &blockName = UNIFORM_BLOCK_NAMES.at(type);

  auto index = glGetUniformBlockIndex(_program, blockName.c_str());
  GLint size = 0;
  glGetActiveUniformBlockiv (_program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
  _uniformBlocks[(int)type] = std::make_unique<UniformBlock>(type, index, size);

  // Setting up binding point
  auto bindingPoint = _uniformBlocks[(int)type]->bindingPoint();
  glUniformBlockBinding(_program, index, bindingPoint);

//  ENGLog("Added UBO binding %s, of size %i. Binding point: %i", blockName.c_str(), size, bindingPoint);

  return getUniformBlock(type);
}

UniformBlock *Shader::getUniformBlock (const UniformBlockName type) {
  return _uniformBlocks[(int)type].get();
}

Uniform *Shader::addUniform(const UniformName type) {
  auto &uniformName = UNIFORM_NAMES.at(type);
  GLint location = glGetUniformLocation(_program, uniformName.c_str());
  if (location == -1) {
    ENGLog("Can't find uniform %s", uniformName.c_str());
  }
  _uniforms[(int)type] = std::make_unique<Uniform>(location, type);
  auto result = this->getUniform(type);

  if (UNIFORM_TEXTURE_BLOCKS.find(type) != UNIFORM_TEXTURE_BLOCKS.end()) {
    ENGLog("Added default uniform texture value for %s = %i", uniformName.c_str(), UNIFORM_TEXTURE_BLOCKS.at(type));
    bind();
    result->setInt(UNIFORM_TEXTURE_BLOCKS.at(type));
  }

  return result;
}

Uniform *Shader::getUniform (const UniformName type) {
  return _uniforms[(int)type].get();
}

void Shader::setupUniformsForCaps(ShaderCapsSetPtr caps) {
  std::unordered_set<UniformName> uniforms;
  std::unordered_set<UniformBlockName> uniformBlocks;

  // Auto adding uniforms and blocks
  for (auto &iterator : UNIFORMS_PER_CAP) {
    if (caps->hasCap(iterator.first)) {
      auto &config = UNIFORMS_PER_CAP.at(iterator.first);
      for (auto &name : config.uniforms) {
        uniforms.insert(name);
      }
      for (auto &name : config.uniformBlocks) {
        uniformBlocks.insert(name);
      }
    }
  }

  for (auto name : uniforms) {
    addUniform(name);
  }

  for (auto name : uniformBlocks) {
    addUniformBlock(name);
  }
}

