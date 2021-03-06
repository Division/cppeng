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
#include "objects/Projector.h"
#include "render/debug/DebugDraw.h"

const unsigned int SHADOW_ATLAS_SIZE = 4096;

SceneRenderer::SceneRenderer() {
  _debugDraw = std::make_shared<DebugDraw>();
  _depthPrePass = std::make_shared<RenderPass>();
  _depthPrePass->mode(RenderMode::DepthOnly);
  _mainPass = std::make_shared<RenderPass>();
  _mainPass->mode(RenderMode::Normal);
  _2dPass = std::make_shared<RenderPass>();
  _2dPass->mode(RenderMode::UI);
  _renderer = std::make_shared<Renderer>(_debugDraw);
  _postEffect = std::make_unique<PostEffect>(_renderer);
  _shadowMap = std::make_unique<ShadowMap>(SHADOW_ATLAS_SIZE, SHADOW_ATLAS_SIZE, _renderer);
}

void SceneRenderer::renderScene(ScenePtr scene, ICameraParamsProviderPtr camera, ICameraParamsProviderPtr camera2D) const {
  if (!camera) { return; }

  // Shadow casters
  auto &visibleLights = scene->visibleLights(camera);
  _shadowCasters.clear();
  for (auto &light : visibleLights) {
    if (light->castShadows()) {
      _shadowCasters.push_back(std::static_pointer_cast<IShadowCaster>(light));
    }
  }

  auto &visibleProjectors = scene->visibleProjectors(camera);
  for (auto &projector : visibleProjectors) {
    if (projector->castShadows()) {
      _shadowCasters.push_back(std::static_pointer_cast<IShadowCaster>(projector));
    }
  }

  _shadowMap->setupShadowCasters(_shadowCasters);

  _renderer->setupBuffers(scene, camera, camera2D);

  _shadowMap->renderShadowMaps(_shadowCasters, scene);
  //

  // Switching to main framebuffer
  if (!_mainFrameBuffer) {
    auto viewSize = camera->cameraViewSize();
    _mainFrameBuffer = std::make_shared<SwappableFrameBufferObject>(viewSize.x, viewSize.y, true, true);
  }

  _mainFrameBuffer->swap();
  _mainFrameBuffer->current()->bind();
  glDepthMask(GL_TRUE);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //

  engine::Performance::startTimer(engine::Performance::Entry::DepthPrePass);
    _renderer->clearQueues();
    auto renderer = std::static_pointer_cast<IRenderer>(_renderer);
    _debugDraw->render(renderer); // Should go BEFORE populateQueues because
    _depthPrePass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
    _renderer->populateQueues(scene, camera); // populateQueues also uploads transform UBO and debug objects should be included

    // Depth pre pass
  _renderer->renderScene(_depthPrePass);
  engine::Performance::stopTimer(engine::Performance::Entry::DepthPrePass);
  //

  // Main pass
  engine::Performance::startTimer(engine::Performance::Entry::MainPass);
    _mainPass->camera(std::static_pointer_cast<ICameraParamsProvider>(camera));
  _renderer->renderScene(_mainPass);
  engine::Performance::stopTimer(engine::Performance::Entry::MainPass);
  //

  // Post effect
  _mainFrameBuffer->current()->unbind();
  _postEffect->render(_mainFrameBuffer->current()->colorBuffer(), camera);
  //

  // 2D pass
  if (camera2D) {
    _2dPass->camera(camera2D);
    _renderer->renderScene(_2dPass);
  }
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

TexturePtr SceneRenderer::depthTexture() {
  if (!_mainFrameBuffer) { return nullptr; }
  return _mainFrameBuffer->current()->depthBuffer();
}

TexturePtr SceneRenderer::shadowMapDepthTexture() {
  return _shadowMap->depthAtlas();
}

float SceneRenderer::shadowAtlasSize() {
  return SHADOW_ATLAS_SIZE;
}

unsigned int SceneRenderer::ropCount() const {
  return _renderer->ropCount();
}


