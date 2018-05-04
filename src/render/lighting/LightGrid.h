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

struct LightGridCell {
  int offset;
  std::vector<LightObjectPtr> pointLights;
  std::vector<LightObjectPtr> spotLights;
};

class LightGrid {
public:
  typedef std::vector<LightObjectPtr> LightList;
  explicit LightGrid(unsigned int cellSize = 32);
  ~LightGrid();

  void update(unsigned int screenWidth, unsigned int screenHeight);
  void appendLights(const LightList *lights, const CameraPtr camera);
  void upload();
  void bindBufferTextures();

private:
#if ENGINE_USE_BUFFER_TEXTURE
  std::unique_ptr<SwappableTextureBufferObject> _lightIndex;
  std::unique_ptr<SwappableTextureBufferObject> _lightGrid;
#else
  std::unique_ptr<SwappableTexture2DBuffer> _lightIndex;
  std::unique_ptr<SwappableTexture2DBuffer> _lightGrid;
#endif

  unsigned int _cellSize;
  unsigned int _cellsX = 0;
  unsigned int _cellsY = 0;
  std::vector<LightGridCell> _cells;
  unsigned int _lightGridBlock;
  unsigned int _lightIndexBlock;

private:
  LightGridCell *_getCellByXY(int x, int y) { return &_cells[x + y * _cellsX]; };
  void _clearCells();
  void _appendLight(const LightObjectPtr light, const CameraPtr camera);

};


#endif //CPPWRAPPER_LIGHTGRID_H
