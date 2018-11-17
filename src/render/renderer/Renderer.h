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
#include <system/Window.h>
#include "UBOManager.h"
#include "EngMath.h"

class Camera;
class Scene;
class Material;
class LightGrid;
class Window;
class DebugDraw;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

// TODO: refactor into PassRenderer, render into offscreen framebuffer
class Renderer : IRenderer {
public:
  Renderer(Window *window);
  ~Renderer();
  void setupShaders();

  GlobalState state;

  ShaderGenerator *generator() { return &_generator; } // TODO: move to another place
  ShaderPtr getShaderWithCaps (ShaderCapsSetPtr caps) const;
  std::shared_ptr<DebugDraw> debugDraw() const { return _debugDraw; }

  // Rendering
  void renderScene(Scene &scene);
  void postUpdate(float dt);
  const vec4 viewport() const { return _window->viewport(); }

  void projectorTexture(const TexturePtr texture) { _projectorTexture = texture; }
  TexturePtr projectorTexture() const { return _projectorTexture; }

  // IRenderer
  void addRenderOperation(RenderOperation &rop, RenderQueue renderQueue) override;
  void renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform, GLenum mode) override;

private:
  mutable ShaderGenerator _generator;
  mutable std::unordered_map<ShaderCapsSet::Bitmask, ShaderPtr> _shaders;
  std::shared_ptr<DebugDraw> _debugDraw;

  Window *_window;

  std::unique_ptr<UBOManager> _uboManager;
  std::unique_ptr<LightGrid> _lightGrid;
  std::shared_ptr<Texture> _projectorTexture; // usually a spritesheet
  unsigned int _projectorTextureUniform = 0;

  std::vector<RenderOperation> _queues[(int)RenderQueue::Count];
  unsigned int _ropCounter;

private:
  void _prepareQueues(Scene &scene, std::shared_ptr<Camera> camera);
  void _processRenderPipeline();
  void _renderCamera(Scene &scene, std::shared_ptr<Camera> camera);
  void setupMaterialBindings(RenderOperation *rop);
  void _clearQueues();
};


#endif //CPPWRAPPER_RENDERER_H
