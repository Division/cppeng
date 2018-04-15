//
// Created by Sidorenko Nikita on 4/13/18.
//

#ifndef CPPWRAPPER_UBOMANAGER_H
#define CPPWRAPPER_UBOMANAGER_H

#include "render/buffer/VertexBufferObject.h"
#include "render/material/Material.h"
#include <memory>

class UBOManager {
public:
  UBOManager();
  void processMeterialBindings(MaterialPtr material);
  void swap();
  void upload();
  void setupForRender(MaterialPtr material);
private:
  SwappableVertexBufferObjectUniquePtr _transform;
};


#endif //CPPWRAPPER_UBOMANAGER_H
