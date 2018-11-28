//
// Created by Sidorenko Nikita on 11/20/18.
//

#ifndef CPPWRAPPER_SCENERENDERER_H
#define CPPWRAPPER_SCENERENDERER_H

#include <memory>
#include "View.h"
#include "IRenderer.h"

class Scene;
typedef std::shared_ptr<Scene> ScenePtr;

class Texture;
class UBOManager;
class LightGrid;
class Renderer;
class DebugDraw;
class SwappableFrameBufferObject;
class PostEffect;

class SceneRenderer {
public:
  SceneRenderer();
  ~SceneRenderer();

  void projectorTexture(const TexturePtr texture);
  std::shared_ptr<DebugDraw> debugDraw() const;
  void renderScene(ScenePtr scene) const;

private:
  mutable std::shared_ptr<Texture> _tex;
  std::shared_ptr<Renderer> _renderer;
  std::shared_ptr<View> _depthPrePass;
  std::shared_ptr<View> _mainPass;
  std::unique_ptr<PostEffect> _postEffect;

  mutable std::shared_ptr<SwappableFrameBufferObject> _mainFrameBuffer = nullptr;
};


#endif //CPPWRAPPER_SCENERENDERER_H
