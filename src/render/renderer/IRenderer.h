//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_IRENDERER_H
#define CPPWRAPPER_IRENDERER_H

#include "EngMath.h"

class Mesh;

class IRenderer {
public:
  virtual void renderMesh(Mesh &mesh, const mat4 &transform) = 0;
};

#endif //CPPWRAPPER_IRENDERER_H
