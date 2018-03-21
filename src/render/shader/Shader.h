//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_SHADER_H
#define CPPWRAPPER_SHADER_H

#include "SDL_opengl.h"
#include "Uniform.h"
#include <common/ICleanableObserver.h>
#include <common/ICleanable.h>
#include <vector>
#include <string>
#include <map>

class Uniform;

enum class ShaderAttrib : int {
  Position = 0,
  TexCoord0,
  TexCoord1
};

extern const std::map<ShaderAttrib, std::string> SHADER_ATTRIB_NAMES;


class Shader
{
public:
  Shader(const std::string &vertexSource, const std::string &fragmentSource);
  virtual void notifyDirty(ICleanable *uniform);

  void bind();

  Uniform *addUniform (UniformType type);
  Uniform *getUniform(UniformType type);

private:
  GLuint _compileShader(const std::string &vertexSource, const std::string &fragmentSource);
  GLuint _loadShader(const std::string &source, GLenum type);
  void _setupAttribs(GLuint program);
private:
  std::unique_ptr<Uniform> _uniforms[(int)UniformType::Count];
  GLuint _program;
  std::vector<ICleanable *> _dirtyUniforms;


};


#endif //CPPWRAPPER_SHADER_H
