//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_IRENDERER_H
#define CPPWRAPPER_IRENDERER_H

#include "EngMath.h"
#include "RenderOperation.h"

class Mesh;
class Material;

enum class RenderMode : int {
  DepthOnly = 0,
  Normal, // Depth test, no depth write,
  NormalDepthWrite // To use without depth pre pass
};

enum class RenderQueue : int {
  Opaque = 0,
  Translucent,
  Debug,
  Count
};

class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual void addRenderOperation(RenderOperation &rop, RenderQueue queue) = 0;
  virtual void renderMesh(MeshPtr mesh, MaterialPtr material, const mat4 &transform, GLenum mode) = 0;
};

#endif //CPPWRAPPER_IRENDERER_H
