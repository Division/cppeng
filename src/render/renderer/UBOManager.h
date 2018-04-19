//
// Created by Sidorenko Nikita on 4/13/18.
//

#ifndef CPPWRAPPER_UBOMANAGER_H
#define CPPWRAPPER_UBOMANAGER_H

#include "render/buffer/VertexBufferObject.h"
#include "render/material/Material.h"
#include <memory>
#include "objects/LightObject.h"

class UBOManager {
public:
  UBOManager();
  void processMeterialBindings(MaterialPtr material);
  void swap();
  void upload();
  void setupForRender(MaterialPtr material);
  void updateLights(const std::vector<LightObjectPtr> *lights);
private:
  SwappableVertexBufferObjectUniquePtr _transform;
  SwappableVertexBufferObjectUniquePtr _light;

};


#endif //CPPWRAPPER_UBOMANAGER_H
