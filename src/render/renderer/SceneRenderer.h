//
// Created by Sidorenko Nikita on 11/20/18.
//

#ifndef CPPWRAPPER_SCENERENDERER_H
#define CPPWRAPPER_SCENERENDERER_H

#include <memory>
#include "RenderPass.h"
#include "IRenderer.h"
#include "render/shading/IShadowCaster.h"
#include <vector>

class Scene;
typedef std::shared_ptr<Scene> ScenePtr;

class Texture;
class UBOManager;
class LightGrid;
class Renderer;
class DebugDraw;
class SwappableFrameBufferObject;
class PostEffect;
class ShadowMap;

class SceneRenderer {
public:
  static float shadowAtlasSize();

  SceneRenderer();
  ~SceneRenderer();

  void projectorTexture(const TexturePtr texture);
  std::shared_ptr<DebugDraw> debugDraw() const;
  void renderScene(ScenePtr scene, ICameraParamsProviderPtr camera, ICameraParamsProviderPtr camera2D = nullptr) const;
  TexturePtr depthTexture();
  TexturePtr shadowMapDepthTexture();
  unsigned int ropCount() const;

private:
  std::shared_ptr<DebugDraw> _debugDraw;
  std::shared_ptr<Renderer> _renderer;
  std::shared_ptr<RenderPass> _depthPrePass;
  std::shared_ptr<RenderPass> _mainPass;
  std::shared_ptr<RenderPass> _2dPass;
  std::unique_ptr<PostEffect> _postEffect;
  std::unique_ptr<ShadowMap> _shadowMap;

  mutable std::shared_ptr<SwappableFrameBufferObject> _mainFrameBuffer = nullptr;
  mutable std::vector<std::shared_ptr<IShadowCaster>> _shadowCasters;
};


#endif //CPPWRAPPER_SCENERENDERER_H
