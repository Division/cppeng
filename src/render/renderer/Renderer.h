//
// Created by Sidorenko Nikita on 2/18/18.
//

#ifndef CPPWRAPPER_RENDERER_H
#define CPPWRAPPER_RENDERER_H

#include "render/shader/ShaderGenerator.h"
#include "render/shader/Shader.h"
#include <unordered_map>
#include "IRenderer.h"
#include <memory>
#include <system/Window.h>
#include "UBOManager.h"
#include "EngMath.h"
#include "ICameraParamsProvider.h"

class Camera;
class Scene;
class Material;
class LightGrid;
class Window;
class DebugDraw;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class Scene;
typedef std::shared_ptr<Scene> ScenePtr;
class View;
typedef std::shared_ptr<View> ViewPtr;


// For now renderer is a single instance per app
// It may change if need to render multiple fully shaded passes
// (each pass would require it's own LightGrid instance)
class Renderer : public IRenderer {
public:
  explicit Renderer();
  ~Renderer() override;

  std::shared_ptr<DebugDraw> debugDraw() const { return _debugDraw; }

  // Rendering
  void renderPrepare();
  void renderScene(std::shared_ptr<Scene> scene, ViewPtr view);

  void projectorTexture(const TexturePtr texture) { _projectorTexture = texture; }
  TexturePtr projectorTexture() const { return _projectorTexture; }

  // IRenderer
  void addRenderOperation(RenderOperation &rop, RenderQueue renderQueue) override;
  void renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform, GLenum mode) override;

private:
  std::shared_ptr<DebugDraw> _debugDraw;
  std::unique_ptr<UBOManager> _uboManager;
  std::unique_ptr<LightGrid> _lightGrid;
  std::shared_ptr<Texture> _projectorTexture; // usually a spritesheet
  unsigned int _projectorTextureUniform = 0;
  std::shared_ptr<Material> _depthPrePassMaterial;

  std::vector<RenderOperation> _queues[(int)RenderQueue::Count];
  unsigned int _ropCounter;

private:
  void _prepareQueues(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
  void _processRenderPipeline(RenderMode mode);
  void _renderCamera(std::shared_ptr<Scene> scene, std::shared_ptr<ICameraParamsProvider> camera);
  void setupAndUploadUBO(RenderOperation *rop);
  void _clearQueues();
};


#endif //CPPWRAPPER_RENDERER_H
