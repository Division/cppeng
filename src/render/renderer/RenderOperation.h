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
#include "render/shader/UniformBufferStruct.h"

struct RenderOperation {
  MeshPtr mesh;
  MaterialPtr material;
  UBOStruct::ObjectParams *objectParams = nullptr;
  UBOStruct::SkinningMatrices *skinningMatrices = nullptr;

  mat4 modelMatrix;
  unsigned int layer = 1u << 0;
  GLenum mode = GL_TRIANGLES;
  unsigned int index;
  MultiVBOAddress objectParamsBlockOffset; // signed int to use -1 as unready marker
  MultiVBOAddress skinningOffset;
  int renderOrder = 0;
  std::string debugInfo;
};

#endif //CPPWRAPPER_RENDEROPERATION_H
