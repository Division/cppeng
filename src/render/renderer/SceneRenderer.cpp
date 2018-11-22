//
// Created by Sidorenko Nikita on 11/20/18.
//

#include "SceneRenderer.h"
#include "scene/Scene.h"
#include "UBOManager.h"
#include <memory>
#include "render/lighting/LightGrid.h"
#include <render/renderer/Renderer.h>
#include "render/renderer/ICameraParamsProvider.h"
#include "objects/Camera.h"

SceneRenderer::SceneRenderer() {
  _depthPrePass = std::make_shared<View>();
  _depthPrePass->mode(RenderMode::DepthOnly);
  _mainPass = std::make_shared<View>();
  _mainPass->mode(RenderMode::Normal);
  _renderer = std::make_unique<Renderer>();
}

void SceneRenderer::renderScene(ScenePtr scene) const {
  // TODO: shadow map rendering first

  auto camera = scene->cameraCount() ? scene->cameras()[0] : nullptr;
  if (!camera) { return; }

  _mainPass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
  _renderer->renderScene(scene, _mainPass);
}

SceneRenderer::~SceneRenderer() {

}

void SceneRenderer::projectorTexture(const TexturePtr texture) {
  _renderer->projectorTexture(texture);
}

std::shared_ptr<DebugDraw> SceneRenderer::debugDraw() const {
  return _renderer->debugDraw();
}

