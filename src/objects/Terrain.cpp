//
// Created by Sidorenko Nikita on 5/9/18.
//

#include "Terrain.h"
#include <fstream>
#include "system/Logging.h"
#include "render/material/MaterialTypes.h"
#include "loader/TextureLoader.h"

void Terrain::loadHeightmap(const std::string &filename) {
  ENGLog("Loading heightmap: %s", filename.c_str());
  std::ifstream stream(filename, std::ios::in | std::ios::binary);
  stream.seekg(0, std::ios::end);
  auto fileSize = stream.tellg();
  int pointCount = (int)fileSize / 2;
  _heightmap.resize(pointCount);
  stream.seekg(0, std::ios::beg);
  stream.read((char *)&_heightmap[0], fileSize);

  auto dimension = (int)sqrtf((float)pointCount); // assume it always square
  _dimensions = ivec2(dimension, dimension);

  _generateMesh();

  ENGLog("Heightmap size: %i", dimension);
}

void Terrain::_generateMesh() {
  _mesh = std::make_shared<Mesh>(true);
  std::vector<vec3> vertices;
  std::vector<GLushort> indices;
  std::vector<vec2> texcoord;
  ivec2 actualDimensions = _dimensions;
  vec3 cellShift = vec3(_size.x / (actualDimensions.x), 0, _size.z / (actualDimensions.y));

  for (int i = 0; i < actualDimensions.y; i++) {
    for (int j = 0; j < actualDimensions.x; j++) {
      int index = i * actualDimensions.x + j;
      float y = _size.y / 65535.0f * _heightmap[index];
      vec3 origin = vec3(j * cellShift.x, y, i * cellShift.z);
      vec2 uv((float)j * _uvRepeat.x / (actualDimensions.x - 1.0f), -(float)i * _uvRepeat.y / (actualDimensions.y - 1.0f));
      vertices.push_back(origin);
      texcoord.push_back(uv);

      if (i < actualDimensions.y - 1 && j < actualDimensions.x - 1) {
        int index0 = index;
        int index1 = index0 + 1;
        int index2 = index0 + actualDimensions.x;
        int index3 = index0 + actualDimensions.x + 1;
        indices.push_back((GLushort)index0);
        indices.push_back((GLushort)index2);
        indices.push_back((GLushort)index1);
        indices.push_back((GLushort)index2);
        indices.push_back((GLushort)index3);
        indices.push_back((GLushort)index1);
      }
    }
  }

  _mesh->setVertices(vertices);
  _mesh->setIndices(indices);
  _mesh->setTexCoord0(texcoord);
  _mesh->calculateNormals();
  _mesh->calculateTBN();
  _mesh->createBuffer();
}

void Terrain::_createMaterial() {
  if (_layers.empty()) {
    return;
  }

//  auto material = std::make_shared<MaterialLighting>();

  auto material = std::make_shared<MaterialTerrain>((int)_layers.size(), (bool)_specularmap);
  for (int i = 0; i < _layers.size(); i++) {
    material->diffuse(_layers[i].diffuse, i);
    material->normalMap(_layers[i].normal, i);
  }

  if (_layers.size() > 1) {
    material->splatmap(_splatmap);
  }

  if (_specularmap) {
    material->specularmap(_specularmap);
  }

  _material = material;
}

void Terrain::addTextures(const std::string &diffuse, const std::string &normalMap) {
  TerrainLayer layer;
  layer.diffuse = loader::loadTexture(diffuse);
  layer.normal = loader::loadTexture(normalMap, false);
  _layers.emplace_back(layer);
}

void Terrain::loadSplatmap(const std::string &filename) {
  _splatmap = loader::loadTexture(filename, false);
}

void Terrain::loadSpecularmap(const std::string &filename) {
  _specularmap = loader::loadTexture(filename, false);
}

void Terrain::render(IRenderer &renderer) {
  if (!_material) {
    _createMaterial();
  }

  MeshObject::render(renderer);
}


