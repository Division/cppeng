//
// Created by Sidorenko Nikita on 2/15/18.
//

#ifndef CPPWRAPPER_DEBUGDRAW_H
#define CPPWRAPPER_DEBUGDRAW_H

#include "EngMath.h"
#include <vector>
#include <memory>
#include "render/renderer/IRenderer.h"

class Mesh;
class MaterialDebug;

class DebugDraw {
public:
  DebugDraw();
  void drawLine(const vec3 &p1, const vec3 &p2, const vec4 &color);
  void drawPoint(const vec3 &p, const vec3 &color, float size = 5.0f);
  void drawFrustum(mat4 viewProjection);

  void render(IRenderer &renderer);
private:
  int _currentMeshIndex = 0;
  std::vector<vec3> _lines;
  std::vector<vec3> _points;
  std::vector<vec4> _lineColors;
  std::vector<vec4> _pointColors;
  std::shared_ptr<Mesh> _lineMeshes[2]; // two meshes to swap
  std::shared_ptr<Mesh> _pointMeshes[2];
  std::shared_ptr<MaterialDebug> _material;

};


#endif //CPPWRAPPER_DEBUGDRAW_H
