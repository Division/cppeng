//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_SHADER_H
#define CPPWRAPPER_SHADER_H

#include "EngineGL.h"
#include "Uniform.h"
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <unordered_set>
#include "ShaderCaps.h"

class Uniform;

//---------------------
// Shader

enum class ShaderAttrib : int {
  Position = 0,
  Normal,
  Bitangent,
  Tangent,
  TexCoord0,
  VertexColor,
  TexCoord1,
  JointWeights,
  JointIndices
};

extern const std::map<ShaderAttrib, std::string> SHADER_ATTRIB_NAMES;

class Shader
{
public:
  Shader(const std::string &vertexSource, const std::string &fragmentSource);

  void bind();
  void unbind();

  UniformBlock *addUniformBlock(UniformBlockName blockName);
  UniformBlock *getUniformBlock(UniformBlockName type);

  Uniform *addUniform (UniformName type);
  Uniform *getUniform(UniformName type);

  void setupUniformsForCaps(ShaderCapsSetPtr caps);

private:
  GLuint _compileShader(const std::string &vertexSource, const std::string &fragmentSource);
  GLuint _loadShader(const std::string &source, GLenum type);
  void _setupAttribs(GLuint program);
private:
  std::unique_ptr<Uniform> _uniforms[(int)UniformName::Count];
  std::unique_ptr<UniformBlock> _uniformBlocks[(int)UniformBlockName::Count];
  GLuint _program;
};

typedef std::shared_ptr<Shader> ShaderPtr;

#endif //CPPWRAPPER_SHADER_H
