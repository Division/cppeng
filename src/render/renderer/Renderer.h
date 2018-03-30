//
// Created by Sidorenko Nikita on 2/18/18.
//

#ifndef CPPWRAPPER_RENDERER_H
#define CPPWRAPPER_RENDERER_H

#include "render/shader/ShaderGenerator.h"
#include "render/shader/Shader.h"
#include <unordered_map>
#include "IRenderer.h"

class Scene;
class Material;

class Renderer : IRenderer {
public:
  Renderer() {};
  void setupShaders();
  ShaderGenerator *generator() { return &_generator; }
  ShaderPtr getShaderWithCaps (ShaderCapsSetPtr caps);

  void renderScene(Scene &scene);

  // IRenderer
  void renderMesh(Mesh &mesh, const Material &material, const mat4 &transform) override;

protected:
  mutable ShaderGenerator _generator;
  mutable std::unordered_map<ShaderCapsSet::Bitmask, ShaderPtr> _shaders;

protected:
  void _processRenderPipeline();
};


#endif //CPPWRAPPER_RENDERER_H
