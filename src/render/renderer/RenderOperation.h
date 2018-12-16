//
// Created by Sidorenko Nikita on 4/13/18.
//

#ifndef CPPWRAPPER_RENDEROPERATION_H
#define CPPWRAPPER_RENDEROPERATION_H

#include "render/mesh/Mesh.h"
#include "EngMath.h"
#include "EngineGL.h"
#include "render/material/Material.h"
#include "render/buffer/MultiVBOAddress.h"

struct RenderOperation {
  MeshPtr mesh;
  MaterialPtr material;
  mat4 modelMatrix;
  GLenum mode = GL_TRIANGLES;
  unsigned int index;
  bool isSkinning = false;
  MultiVBOAddress transformBlockOffset; // signed int to use -1 as unready marker
  MultiVBOAddress skinningOffset;
  int renderOrder;
  std::string debugInfo;

  void reset () {
    mesh = nullptr;
    material = nullptr;
    modelMatrix = mat4();
    index = 0;
    isSkinning = false;
    renderOrder = 0;
    debugInfo = "";
  }
};

#endif //CPPWRAPPER_RENDEROPERATION_H
