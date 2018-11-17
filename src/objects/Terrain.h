//
// Created by Sidorenko Nikita on 5/9/18.
//

#ifndef CPPWRAPPER_TERRAIN_H
#define CPPWRAPPER_TERRAIN_H

#include "EngMath.h"
#include <vector>
#include <string>
#include "MeshObject.h"

struct TerrainLayer {
  TexturePtr diffuse;
  TexturePtr normal;
};

// Simplest terrain class ever
// Reads photoshop .raw heightmap
// Supports splatmap and up to 4 layers of texture + normal.
// No LOD, geomipmapping etc. Just renders all the geometry in a single draw call.
class Terrain : public MeshObject {
public:
  void loadHeightmap(const std::string &filename);
  void addTextures(const std::string &diffuse, const std::string &normalMap);
  void height(float height) { _size.y = height; }
  float height() { return _size.y; }
  void loadSplatmap(const std::string &filename);
  void loadSpecularmap(const std::string &filename);

protected:
  void render(IRenderer &renderer) override;

private:
  vec3 _size = vec3(30, 3, 30);
  vec2 _cellSize;
  vec2 _uvRepeat = vec2(20, 20);
  ivec2 _dimensions;
  std::vector<unsigned short> _heightmap;
  std::vector<TerrainLayer> _layers;
  TexturePtr _splatmap;
  TexturePtr _specularmap;
  void _generateMesh();
  void _createMaterial();
};


#endif //CPPWRAPPER_TERRAIN_H
