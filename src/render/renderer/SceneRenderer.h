//
// Created by Sidorenko Nikita on 11/20/18.
//

#ifndef CPPWRAPPER_SCENERENDERER_H
#define CPPWRAPPER_SCENERENDERER_H

#include <memory>
#import "View.h"
#import "IRenderer.h"

class Scene;
typedef std::shared_ptr<Scene> ScenePtr;

class UBOManager;
class LightGrid;
class Renderer;
class DebugDraw;

class SceneRenderer {
public:
  SceneRenderer();
  ~SceneRenderer();

  void projectorTexture(const TexturePtr texture);
  std::shared_ptr<DebugDraw> debugDraw() const;
  void renderScene(ScenePtr scene) const;

private:
  std::unique_ptr<Renderer> _renderer;
  std::shared_ptr<View> _depthPrePass;
  std::shared_ptr<View> _mainPass;
};


#endif //CPPWRAPPER_SCENERENDERER_H
