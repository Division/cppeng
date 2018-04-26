//
// Created by Sidorenko Nikita on 4/21/18.
//

#include "EngineGL.h"
#include <objects/Camera.h>
#include "LightGrid.h"
#include "objects/LightObject.h"
#include "system/Logging.h"
#include "render/buffer/TextureBufferObject.h"
#include "render/shader/Uniform.h"

struct LightGridStruct {
  unsigned int offset;
  unsigned int pointLightCount;
};

LightGrid::LightGrid(unsigned int cellSize) : _cellSize(cellSize) {
  _lightGrid = std::make_unique<SwappableTextureBufferObject>(GL_RG32UI, GL_DYNAMIC_DRAW);
  _lightIndex = std::make_unique<SwappableTextureBufferObject>(GL_R16, GL_DYNAMIC_DRAW);

  _lightGridBlock = UNIFORM_TEXTURE_BLOCKS.at(UniformName::LightGrid);
  _lightIndexBlock = UNIFORM_TEXTURE_BLOCKS.at(UniformName::LightIndices);
}

LightGrid::~LightGrid() {

}

void LightGrid::update(unsigned int screenWidth, unsigned int screenHeight) {
  auto newCellsX = (unsigned int)ceilf((float)screenWidth / (float)_cellSize);
  auto newCellsY = (unsigned int)ceilf((float)screenHeight / (float)_cellSize);

  if (newCellsX != _cellsX || newCellsY != _cellsY) {
    _cellsX = newCellsX;
    _cellsY = newCellsY;
  }

  auto size = _cellsX * _cellsY;
  if (size > _cells.size()) {
    _cells.resize(_cellsX * _cellsY);
  }

  _clearCells();
}

void LightGrid::_clearCells() {
  for (auto &cell: _cells) {
    cell.pointLights.clear();
    cell.spotLights.clear();
  }
}

void LightGrid::_appendLight(const LightObjectPtr light, const CameraPtr camera) {
  vec3 position = light->transform()->worldPosition();
  float radius = light->radius();

  AABB lightBounds = AABB::fromSphere(
      position, radius).project(camera->viewMatrix(), camera->projectionMatrix(), camera->viewport()
  );

  AABB bounds;
  bounds.min = lightBounds.min;
  bounds.max = lightBounds.min;
  bounds.expand(lightBounds.max);

  auto startX = (int)floorf(fmaxf(bounds.min.x / _cellSize, 0));
  auto startY = (int)floorf(fmaxf(bounds.min.y / _cellSize, 0));
  auto endX = (int)floorf(fminf(bounds.max.x / _cellSize, _cellsX - 1));
  auto endY = (int)floorf(fminf(bounds.max.y / _cellSize, _cellsY - 1));

  if ((endX < 0) || (startX >= (int)_cellsX) || (endY < 0) || (startY >= (int)_cellsY)) {
    return; // light out of grid bounds
  }

  for (int i = startX; i <= endX; i++ ) {
    for (int j = startY; j <= endY; j++) {
      auto cell = _getCellByXY(i, j);
      cell->pointLights.push_back(light);
    }
  }

//  ENGLog("Light grid %i %i %i %i", startX, startY, endX, endY);
}

void LightGrid::appendLights(const LightList *lights, const CameraPtr camera) {
  for (auto &light : *lights) {
    _appendLight(light, camera);
  }
}

void LightGrid::upload() {
  _lightGrid->swap();
  _lightIndex->swap();

  auto gridBuffer = _lightGrid->current();
  auto indexBuffer = _lightIndex->current();

  gridBuffer->resize((unsigned int)_cells.size() * sizeof(LightGridStruct));
  auto gridBufferPointer = (LightGridStruct *)gridBuffer->bufferPointer();

  unsigned int currentOffset = 0;
  for (int i = 0; i < _cells.size(); i++) {
    auto &cell = _cells[i];
    gridBufferPointer[i].offset = currentOffset;
    gridBufferPointer[i].pointLightCount = (unsigned int)cell.pointLights.size();
//    gridBufferPointer[i].offset = 1;
//    gridBufferPointer[i].pointLightCount = 1;
//    continue;


    int indexDataSize = gridBufferPointer[i].pointLightCount /* + gridBufferPointer[i].spotLightCount */;
    indexBuffer->resize((indexDataSize + currentOffset) * sizeof(unsigned short));
    // pointer should be obtained after resize() since resize may reallocate the data
    auto indexBufferPointer = (unsigned short *)indexBuffer->bufferPointer();

    for (int j = 0; j < gridBufferPointer[i].pointLightCount; j++) {
      indexBufferPointer[currentOffset + j] = (unsigned short)cell.pointLights[j]->index();
    }

    currentOffset += gridBufferPointer[i].pointLightCount;

    /*
    for (int j = 0; j < gridBufferPointer[i].spotLightCount; j++) {
      indexBufferPointer[currentOffset + j] = (unsigned short)cell.spotLights[j]->index();
    }

    currentOffset += gridBufferPointer[i].spotLightCount; */
  }

  gridBuffer->setDirty();
  gridBuffer->upload();
  indexBuffer->setDirty();
  indexBuffer->upload();
}

void LightGrid::bindBufferTextures() {
  _lightGrid->current()->bindTexture(_lightGridBlock);
  _lightIndex->current()->bindTexture(_lightIndexBlock);
}


