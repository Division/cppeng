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

class SceneRenderer {
public:
  SceneRenderer();

  ~SceneRenderer();

  void renderScene(ScenePtr scene) const;

private:
  std::unique_ptr<UBOManager> _uboManager;
  std::unique_ptr<LightGrid> _lightGrid;
  View _depthPrePass;
  View _mainPass;
};


#endif //CPPWRAPPER_SCENERENDERER_H
