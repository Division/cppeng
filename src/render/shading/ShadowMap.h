//
// Created by Sidorenko Nikita on 2018-11-28.
//

#ifndef CPPWRAPPER_SHADOWMAP_H
#define CPPWRAPPER_SHADOWMAP_H

#include <memory>
#include "EngTypes.h"
#include "EngMath.h"
#include "render/shading/IShadowCaster.h"
#include <vector>

class Renderer;

class ShadowMap {
public:
  ShadowMap(unsigned int resolutionX, unsigned int resolutionY, std::shared_ptr<Renderer> renderer);

  void renderShadowMaps(const std::vector<IShadowCasterPtr> &shadowCasters, const ScenePtr &scene);
private:
  uvec2 _resolution;
  uvec2 _cellPixelSize;
  vec2 _cellSize;
  unsigned int _pixelSpacing = 2;
  FrameBufferObjectPtr _depthAtlas;
  std::shared_ptr<Renderer> _renderer;

private:
  Rect getCellPixelRect(unsigned int index);
  Rect getCellRect(unsigned int index);
};


#endif //CPPWRAPPER_SHADOWMAP_H
