//
// Created by Sidorenko Nikita on 2018-11-28.
//

#include "ShadowMap.h"
#include "render/buffer/FrameBufferObject.h"
#include "render/renderer/Renderer.h"
#include "IShadowCaster.h"
#include "render/renderer/View.h"

const unsigned int CELL_COUNT = 8;
const auto MAX_MAPS = CELL_COUNT * CELL_COUNT;

ShadowMap::ShadowMap(unsigned int resolutionX, unsigned int resolutionY, std::shared_ptr<Renderer> renderer) {
  _resolution = uvec2(resolutionX, resolutionY);
  _renderer = renderer;
  _depthAtlas = std::make_shared<FrameBufferObject>(resolutionX, resolutionY, false, true);

  float emptySpacing = (CELL_COUNT - 1) * _pixelSpacing;
  _cellPixelSize = glm::floor(vec2(resolutionX - emptySpacing, resolutionY - emptySpacing) / (float)CELL_COUNT);
  _cellSize = vec2(_cellPixelSize) / vec2(_resolution);
}

void ShadowMap::renderShadowMaps(const std::vector<IShadowCasterPtr> &shadowCasters, const ScenePtr &scene) {
  _depthAtlas->bind();
  glClear(GL_DEPTH_BUFFER_BIT);

  auto view = std::make_shared<View>();
  view->mode(RenderMode::DepthOnly);

  unsigned int index = 0;
  for (auto &caster : shadowCasters) {
    if (index >= MAX_MAPS) {
      caster->viewport(vec4(0,0,0,0));
      continue;
    }

    vec4 viewport = (vec4)getCellPixelRect(index);
    caster->viewport(viewport);
    view->camera(caster);
    _renderer->renderScene(scene, view);
    index++;
  }

  _depthAtlas->unbind();
}

Rect ShadowMap::getCellPixelRect(unsigned int index) {
  unsigned int x = index % CELL_COUNT * (_cellPixelSize.x + _pixelSpacing);
  unsigned int y = index / CELL_COUNT * (_cellPixelSize.y + _pixelSpacing);

  return Rect(x, y, _cellPixelSize.x, _cellPixelSize.y);
}

Rect ShadowMap::getCellRect(unsigned int index) {
  int x = index % CELL_COUNT;
  int y = index / CELL_COUNT;

  vec2 origin = vec2(x, y) * (vec2(_cellPixelSize) + (float)_pixelSpacing) / vec2(_resolution);

  return Rect(origin.x, origin.y, _cellSize.x, _cellSize.y);
}