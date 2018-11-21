//
// Created by Sidorenko Nikita on 11/20/18.
//

#include "SceneRenderer.h"
#include "scene/Scene.h"
#include "UBOManager.h"
#include <memory>
#include "render/lighting/LightGrid.h"

SceneRenderer::SceneRenderer() :
  _depthPrePass(false),
  _mainPass(true) {
  _uboManager = std::make_unique<UBOManager>();
  _lightGrid = std::make_unique<LightGrid>();
}

void SceneRenderer::renderScene(ScenePtr scene) const {
  // TODO: shadow map rendering first

}

SceneRenderer::~SceneRenderer() {

}

