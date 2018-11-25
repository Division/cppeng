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
#include "utils/Performance.h"

SceneRenderer::SceneRenderer() {
  _depthPrePass = std::make_shared<View>();
  _depthPrePass->mode(RenderMode::DepthOnly);
  _mainPass = std::make_shared<View>();
  _mainPass->mode(RenderMode::Normal);
  _renderer = std::make_unique<Renderer>();
}

void SceneRenderer::renderScene(ScenePtr scene) const {
  _renderer->renderPrepare();

  // TODO: shadow map rendering first

  auto camera = scene->cameraCount() ? scene->cameras()[0] : nullptr;
  if (!camera) { return; }

  engine::Performance::startTimer(engine::Performance::Entry::DepthPrePass);
  _depthPrePass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
  _renderer->renderScene(scene, _depthPrePass);
  engine::Performance::stopTimer(engine::Performance::Entry::DepthPrePass);

  engine::Performance::startTimer(engine::Performance::Entry::MainPass);
  _mainPass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
  _renderer->renderScene(scene, _mainPass);
  engine::Performance::stopTimer(engine::Performance::Entry::MainPass);
}

SceneRenderer::~SceneRenderer() {

}

void SceneRenderer::projectorTexture(const TexturePtr texture) {
  _renderer->projectorTexture(texture);
}

std::shared_ptr<DebugDraw> SceneRenderer::debugDraw() const {
  return _renderer->debugDraw();
}

