//
// Created by Sidorenko Nikita on 11/20/18.
//

#include "SceneRenderer.h"
#include "scene/Scene.h"
#include "UBOManager.h"
#include <memory>
#include "render/shading/LightGrid.h"
#include <render/renderer/Renderer.h>
#include "render/renderer/ICameraParamsProvider.h"
#include "objects/Camera.h"
#include "utils/Performance.h"
#include "render/buffer/FrameBufferObject.h"
#include "render/texture/Texture.h"
#include "render/effect/PostEffect.h"
#include "render/shading/ShadowMap.h"
#include "objects/LightObject.h"
#include "render/debug/DebugDraw.h"

const unsigned int SHADOW_ATLAS_SIZE = 4096;

SceneRenderer::SceneRenderer() {
  _debugDraw = std::make_shared<DebugDraw>();
  _depthPrePass = std::make_shared<View>();
  _depthPrePass->mode(RenderMode::DepthOnly);
  _mainPass = std::make_shared<View>();
  _mainPass->mode(RenderMode::Normal);
  _renderer = std::make_shared<Renderer>(_debugDraw);
  _postEffect = std::make_unique<PostEffect>(_renderer);
  _shadowMap = std::make_unique<ShadowMap>(SHADOW_ATLAS_SIZE, SHADOW_ATLAS_SIZE, _renderer);
}

void SceneRenderer::renderScene(ScenePtr scene) const {
  _renderer->renderPrepare();

  auto camera = scene->cameraCount() ? scene->cameras()[0] : nullptr;
  if (!camera) { return; }

  // Shadow maps
  auto &visibleLight = scene->visibleLights(camera);
  _shadowCasters.clear();
  for (auto &light : visibleLight) {
    if (light->castShadows()) {
      _shadowCasters.push_back(std::static_pointer_cast<IShadowCaster>(light));
    }
  }

  _shadowMap->renderShadowMaps(_shadowCasters, scene);
  //

  // Switching to main framebuffer
  if (!_mainFrameBuffer) {
    auto viewSize = camera->cameraViewSize();
    _mainFrameBuffer = std::make_shared<SwappableFrameBufferObject>(viewSize.x, viewSize.y, true, true);
  }

  _mainFrameBuffer->swap();
  _mainFrameBuffer->current()->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //

  // Depth pre pass
  engine::Performance::startTimer(engine::Performance::Entry::DepthPrePass);
    _depthPrePass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
    _renderer->renderScene(scene, _depthPrePass);
  engine::Performance::stopTimer(engine::Performance::Entry::DepthPrePass);
  //

  // Main pass
  engine::Performance::startTimer(engine::Performance::Entry::MainPass);
    _mainPass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
    _renderer->renderScene(scene, _mainPass);
  engine::Performance::stopTimer(engine::Performance::Entry::MainPass);
  //


  // Post effect
  _mainFrameBuffer->current()->unbind();
  _postEffect->render(_mainFrameBuffer->current()->colorBuffer(), camera);
  //
}

SceneRenderer::~SceneRenderer() {

}

void SceneRenderer::projectorTexture(const TexturePtr texture) {
  _renderer->projectorTexture(texture);
}

std::shared_ptr<DebugDraw> SceneRenderer::debugDraw() const {
  return _renderer->debugDraw();
}


