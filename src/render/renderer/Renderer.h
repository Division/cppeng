//
// Created by Sidorenko Nikita on 2/18/18.
//

#ifndef CPPWRAPPER_RENDERER_H
#define CPPWRAPPER_RENDERER_H

#include "render/shader/ShaderGenerator.h"
#include "render/shader/Shader.h"
#include <unordered_map>
#include "IRenderer.h"
#include "GlobalState.h"
#include <memory>
#include "UBOManager.h"

class Camera;
class Scene;
class Material;

class Renderer : IRenderer {
public:
  Renderer();
  void setupShaders();

  GlobalState state;

  ShaderGenerator *generator() { return &_generator; }
  ShaderPtr getShaderWithCaps (ShaderCapsSetPtr caps) const;

  void renderScene(Scene &scene);

  // IRenderer
  void addRenderOperation(RenderOperation &rop, RenderQueue renderQueue) override;
  void renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform) override;

protected:
  mutable ShaderGenerator _generator;
  mutable std::unordered_map<ShaderCapsSet::Bitmask, ShaderPtr> _shaders;
  std::unique_ptr<UBOManager> _uboManager;
  std::vector<RenderOperation> _queues[(int)RenderQueue::Count];
  unsigned int _ropCounter;

protected:
  void _prepareQueues();
  void _processRenderPipeline();
  void _renderCamera(Scene &scene, std::shared_ptr<Camera> camera);
  void setupMaterialBindings(MaterialPtr &material, const mat4 &transform);
  void _clearQueues();
};


#endif //CPPWRAPPER_RENDERER_H
