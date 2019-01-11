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

  GLenum mode = GL_TRIANGLES;
  bool depthTest = true;
  MultiVBOAddress objectParamsBlockOffset;
  MultiVBOAddress skinningOffset;
  std::string *debugInfo = nullptr;
};

#endif //CPPWRAPPER_RENDEROPERATION_H
