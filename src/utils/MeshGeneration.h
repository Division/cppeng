//
// Created by Sidorenko Nikita on 4/19/18.
//

#ifndef CPPWRAPPER_MESHGENERATION_H
#define CPPWRAPPER_MESHGENERATION_H

#include "EngMath.h"
#include "EngineGL.h"
#include <vector>
#include "render/mesh/Mesh.h"

namespace MeshGeneration {

  void generateSphere(MeshPtr mesh, int parallelCount, int meridianCount, float radius = 1.0f);
  void generateBox(MeshPtr mesh, float sizeX, float sizeY, float sizeZ);
};


#endif //CPPWRAPPER_MESHGENERATION_H
