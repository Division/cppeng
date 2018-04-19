//
// Created by Sidorenko Nikita on 4/19/18.
//

#include "MeshGeneration.h"

void MeshGeneration::generateSphere(MeshPtr mesh, int parallelCount, int meridianCount, float radius) {
  std::vector<vec3> vertices;
  std::vector<GLushort> indices;

  for (int j = 0; j < parallelCount; j++) {
    float parallel = M_PI * (float)j / (float)(parallelCount - 1);

    for (int i = 0; i < meridianCount; i++) {
      float meridian = 2.0 * M_PI * (float)i / (float)meridianCount;
      float x = radius * sinf(parallel) * cosf(meridian);
      float z = radius * cosf(parallel);

      vertices.push_back(vec3(radius * sinf(parallel) * cosf(meridian),
                         radius * sinf(parallel) * sinf(meridian),
                         radius * cosf(parallel)));

      indices.push_back(j * parallelCount + i);
      indices.push_back(((j + 1) % parallelCount) * parallelCount + i);
      indices.push_back(((j + 1) % parallelCount) * parallelCount + (i + 1) % meridianCount);

      indices.push_back(((j + 1) % parallelCount) * parallelCount + (i + 1) % meridianCount);
      indices.push_back(((j) % parallelCount) * parallelCount + (i + 1) % meridianCount);
      indices.push_back(j * parallelCount + i);
    }
  }

  mesh->setVertices(vertices);
  mesh->setIndices(indices);
}

void MeshGeneration::generateBox(MeshPtr mesh, float sizeX, float sizeY, float sizeZ) {
  auto halfX = sizeX * 0.5f;
  auto halfY = sizeY * 0.5f;
  auto halfZ = sizeZ * 0.5f;

  float srcVertices[] = {
    -halfX, -halfY, -halfZ,
    halfX, -halfY, -halfZ,
    halfX, -halfY, halfZ,
    -halfX, -halfY, halfZ,
    -halfX, halfY, -halfZ,
    halfX, halfY, -halfZ,
    halfX, halfY, halfZ,
    -halfX, halfY, halfZ,
  };

  int srcIndices[] = {
    3, 0, 2, 0, 1, 2, // +y
    4, 7, 6, 4, 6, 5, // -y
    7, 4, 0, 7, 0, 3, // -x
    5, 2, 1, 5, 6, 2, // +x
    4, 5, 1, 4, 1, 0, // -z
    7, 3, 6, 6, 3, 2  // +z
  };

  std::vector<vec3> vertices;

  // duplicate vertices
  for (int i = 0; i < 36; i++) {
    vertices.emplace_back(vec3(srcVertices[srcIndices[i] * 3],
                            srcVertices[srcIndices[i] * 3 + 1],
                            srcVertices[srcIndices[i] * 3 + 2]));
  }

  mesh->setVertices(vertices);
}