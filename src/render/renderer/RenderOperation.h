//
// Created by Sidorenko Nikita on 4/13/18.
//

#ifndef CPPWRAPPER_RENDEROPERATION_H
#define CPPWRAPPER_RENDEROPERATION_H

#include "render/mesh/Mesh.h"
#include "EngMath.h"
#include "EngineGL.h"
#include "render/material/Material.h"

struct RenderOperation {
  MeshPtr mesh;
  MaterialPtr material;
  mat4 modelMatrix;
  GLenum mode = GL_TRIANGLES;
  unsigned int index;
  int transformBlockOffset; // signed int to use -1 as unready marker
  int renderOrder;
  std::string debugInfo;

  void reset () {
    mesh = nullptr;
    material = nullptr;
    modelMatrix = mat4();
    index = 0;
    renderOrder = 0;
    debugInfo = "";
  }
};

#endif //CPPWRAPPER_RENDEROPERATION_H
