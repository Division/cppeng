//
// Created by Sidorenko Nikita on 2/15/18.
//

#include "DebugDraw.h"
#include "EngineGL.h"
#include "render/mesh/Mesh.h"
#include "render/material/MaterialTypes.h"
#include "EngMath.h"

DebugDraw::DebugDraw() {
  _lineMeshes[0] = std::make_shared<Mesh>(true, 2, GL_DYNAMIC_DRAW);
  _lineMeshes[1] = std::make_shared<Mesh>(true, 2, GL_DYNAMIC_DRAW);
  _pointMeshes[0] = std::make_shared<Mesh>(true, 1, GL_DYNAMIC_DRAW);
  _pointMeshes[1] = std::make_shared<Mesh>(true, 1, GL_DYNAMIC_DRAW);
}

void DebugDraw::drawLine(const vec3 &p1, const vec3 &p2, const vec4 &color) {
  _lines.push_back(p1);
  _lines.push_back(p2);
  _lineColors.push_back(color);
  _lineColors.push_back(color);
}

void DebugDraw::drawPoint(const vec3 &p, const vec3 &color, float size) {
  _points.push_back(p);
  _pointColors.push_back(vec4(color, size));
}

void DebugDraw::drawFrustum(mat4 viewProjection) {
  auto inv = glm::inverse(viewProjection);
  vec4 quad1[4] = {
      inv * vec4(1, -1, -1, 1),
      inv * vec4(-1, -1, -1, 1),
      inv * vec4(-1, 1, -1, 1),
      inv * vec4(1, 1, -1, 1),
  };

  vec4 quad2[4] = {
      inv * vec4(1, -1, 1, 1),
      inv * vec4(-1, -1, 1, 1),
      inv * vec4(-1, 1, 1, 1),
      inv * vec4(1, 1, 1, 1),
  };

  for (int i = 0; i < 4; i++) {
    drawPoint(quad1[i], vec3(1, 0, 0), 10);
//    drawPoint(quad2[i], vec3(1, 0, 1), 10);
    drawLine(quad1[i], quad1[(i + 1) % 4], vec4(1, 0, 0, 1));
    drawLine(quad2[i], quad2[(i + 1) % 4], vec4(1, 0, 0, 1));
    drawLine(quad1[i], quad2[i], vec4(1, 0, 0, 1));
  }
}

void DebugDraw::render(IRenderer &renderer) {
  if (!_material) { // need move material creation out from constructor since because renderer isn't fully created yet
    _material = std::make_shared<MaterialDebug>();
  }

  _currentMeshIndex = (_currentMeshIndex + 1) % 2;
  auto lineMesh = _lineMeshes[_currentMeshIndex];
  auto pointMesh = _pointMeshes[_currentMeshIndex];

  if (!_lines.empty()) {
    lineMesh->setVertices(_lines);
    lineMesh->setColors(_lineColors);
    lineMesh->createBuffer();
    RenderOperation lineROP;
    lineROP.mesh = lineMesh;
    lineROP.material = _material;
    lineROP.modelMatrix = mat4();
    lineROP.debugInfo = "debug lines";
    lineROP.mode = GL_LINES;
    renderer.addRenderOperation(lineROP, RenderQueue::Debug);
  }

  if (!_points.empty()) {
    pointMesh->setVertices(_points);
    pointMesh->setColors(_pointColors);
    pointMesh->createBuffer();
    RenderOperation pointROP;
    pointROP.mesh = pointMesh;
    pointROP.material = _material;
    pointROP.modelMatrix = mat4();
    pointROP.debugInfo = "debug points";
    pointROP.mode = GL_POINTS;
    renderer.addRenderOperation(pointROP, RenderQueue::Debug);
  }

  _lines.resize(0);
  _points.resize(0);
}

