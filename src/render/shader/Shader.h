//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_SHADER_H
#define CPPWRAPPER_SHADER_H

#include "EngineGL.h"
#include "Uniform.h"
#include <common/ICleanableObserver.h>
#include <common/ICleanable.h>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <unordered_set>

class Uniform;

//---------------------
// Shader Caps

enum class ShaderCaps : int {
  Color = 0,
  VertexColor,
  PointSize,
  Lighting,
  Texture0,
  Texture1,
  NormalMap,
  SpecularMap,
  TerrainLayer0,
  TerrainLayer1,
  TerrainLayer2,
  ProjectedTexture,
};

class ShaderCapsSet {
public:
  typedef unsigned int Bitmask;

  bool hasCap(ShaderCaps cap) const;
  void addCap (ShaderCaps cap);
  void removeCap (ShaderCaps cap);
  Bitmask getBitmask() const;
  const std::unordered_set<int> &caps() { return _caps; }

private:
  mutable Bitmask _bitmask = 0; // cached bitmask
  mutable bool _maskDirty = false;
  std::unordered_set<int> _caps;

private:
  Bitmask _calculateBitMask() const;
};

typedef std::shared_ptr<ShaderCapsSet> ShaderCapsSetPtr;


//---------------------
// Shader

enum class ShaderAttrib : int {
  Position = 0,
  Normal,
  Bitangent,
  Tangent,
  TexCoord0,
  VertexColor,
  TexCoord1
};

extern const std::map<ShaderAttrib, std::string> SHADER_ATTRIB_NAMES;

class Shader
{
public:
  Shader(const std::string &vertexSource, const std::string &fragmentSource);
  virtual void notifyDirty(ICleanable *uniform);

  void bind();

  UniformBlock *addUniformBlock(UniformBlockName blockName);
  UniformBlock *getUniformBlock(UniformBlockName type);

  Uniform *addUniform (UniformName type);
  Uniform *getUniform(UniformName type);

private:
  GLuint _compileShader(const std::string &vertexSource, const std::string &fragmentSource);
  GLuint _loadShader(const std::string &source, GLenum type);
  void _setupAttribs(GLuint program);
private:
  std::unique_ptr<Uniform> _uniforms[(int)UniformName::Count];
  std::unique_ptr<UniformBlock> _uniformBlocks[(int)UniformBlockName::Count];
  GLuint _program;
  std::vector<ICleanable *> _dirtyUniforms;

};

typedef std::shared_ptr<Shader> ShaderPtr;

#endif //CPPWRAPPER_SHADER_H
