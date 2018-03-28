//
// Created by Sidorenko Nikita on 2/18/18.
//

#ifndef CPPWRAPPER_RENDERER_H
#define CPPWRAPPER_RENDERER_H

#include "render/shader/ShaderGenerator.h"
#include "render/shader/Shader.h"
#include <unordered_map>

class Renderer {
public:
  Renderer() {};
  void setupShaders();
  ShaderGenerator *generator() { return &_generator; }
  ShaderPtr getShaderWithCaps (ShaderCapsSetPtr caps);
private:
  ShaderGenerator _generator;
  std::unordered_map<ShaderCapsSet::Bitmask, ShaderPtr> _shaders;
};


#endif //CPPWRAPPER_RENDERER_H
