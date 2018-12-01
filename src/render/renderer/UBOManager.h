//
// Created by Sidorenko Nikita on 4/13/18.
//

#ifndef CPPWRAPPER_UBOMANAGER_H
#define CPPWRAPPER_UBOMANAGER_H

#include <memory>
#include "render/material/Material.h"
#include "EngTypes.h"
#include "RenderOperation.h"

class SwappableVertexBufferObject;
class MultiVertexBufferObject;
class ICameraParamsProvider;

// Class manages uniform buffer objects
// Mainly used to conveniently share uniform data between shaders
// and fast batch data update
class UBOManager {
public:
  UBOManager();
  void setTransformBlock(RenderOperation *rop);
  void swap();
  void upload(bool includeLighting);
  void setupForRender(RenderOperation *rop);
  void updateLights(const std::vector<LightObjectPtr> &lights);
  void updateProjectors(const std::vector<ProjectorPtr> &projectors);
  int appendCamera(std::shared_ptr<ICameraParamsProvider> camera);
  void activateCamera(unsigned int offset);
  void map();
  void unmap();
private:
  // Data related to objects transformation
  std::shared_ptr<MultiVertexBufferObject> _transform;

  // Regular light sources (point, spot, directional)
  std::shared_ptr<SwappableVertexBufferObject> _light;

  // Decals and projective texture light sources
  std::shared_ptr<SwappableVertexBufferObject> _projector;

  // Cameras
  std::shared_ptr<SwappableVertexBufferObject> _camera;

private:
  int _activeCameraOffset = -1;
};


#endif //CPPWRAPPER_UBOMANAGER_H
