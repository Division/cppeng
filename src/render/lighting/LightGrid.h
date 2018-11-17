//
// Created by Sidorenko Nikita on 4/21/18.
//

#ifndef CPPWRAPPER_LIGHTGRID_H
#define CPPWRAPPER_LIGHTGRID_H

#include <vector>
#include "EngTypes.h"
#include "EngMath.h"
#include <memory>

class SwappableTextureBufferObject;
class SwappableTexture2DBuffer;
class DebugDraw;

struct LightGridCell {
  int offset;
  std::vector<LightObjectPtr> pointLights;
  std::vector<LightObjectPtr> spotLights;
  std::vector<ProjectorPtr> projectors;
  std::vector<ProjectorPtr> decals;
};

class LightGrid {
public:
  explicit LightGrid(unsigned int cellSize = 32);
  ~LightGrid() = default;

  void update(unsigned int screenWidth, unsigned int screenHeight);
  void appendLights(const std::vector<LightObjectPtr> *lights, const CameraPtr camera);
  void appendProjectors(const std::vector<ProjectorPtr> *projectors, const CameraPtr camera);
  void upload();
  void bindBufferTextures();
  void setDebugDraw(std::shared_ptr<DebugDraw> debugDraw);

private:
#if ENGINE_USE_BUFFER_TEXTURE
  std::shared_ptr<SwappableTextureBufferObject> _lightIndex;
  std::shared_ptr<SwappableTextureBufferObject> _lightGrid;
#else
  std::shared_ptr<SwappableTexture2DBuffer> _lightIndex;
  std::shared_ptr<SwappableTexture2DBuffer> _lightGrid;
#endif

  unsigned int _cellSize;
  unsigned int _cellsX = 0;
  unsigned int _cellsY = 0;
  std::vector<LightGridCell> _cells;
  unsigned int _lightGridBlock;
  unsigned int _lightIndexBlock;
  std::shared_ptr<DebugDraw> _debugDraw;

private:
  LightGridCell *_getCellByXY(int x, int y) { return &_cells[x + y * _cellsX]; };
  void _clearCells();
  void _appendItem(const CameraPtr camera, const std::vector<vec3> &edgePoints, std::function<void (LightGridCell *)> callback);

  // Temporary vector to pass data through the functions
  // Placed here instead of the stack to reduce heap allocations
  std::vector<vec3> _lightEdges;
};


#endif //CPPWRAPPER_LIGHTGRID_H
