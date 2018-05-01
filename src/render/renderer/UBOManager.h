//
// Created by Sidorenko Nikita on 4/13/18.
//

#ifndef CPPWRAPPER_UBOMANAGER_H
#define CPPWRAPPER_UBOMANAGER_H

#include <memory>
#include "render/material/Material.h"
#include "objects/LightObject.h"
#include "EngTypes.h"

class SwappableVertexBufferObject;

class UBOManager {
public:
  UBOManager();
  void processMeterialBindings(MaterialPtr material);
  void swap();
  void upload();
  void setupForRender(MaterialPtr material);
  void updateLights(const std::vector<LightObjectPtr> *lights);
  void setCamera(CameraPtr camera);
private:
  std::shared_ptr<SwappableVertexBufferObject> _transform;
  std::shared_ptr<SwappableVertexBufferObject> _light;
  std::shared_ptr<SwappableVertexBufferObject> _camera;

};


#endif //CPPWRAPPER_UBOMANAGER_H
