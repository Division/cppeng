//
// Created by Sidorenko Nikita on 4/21/18.
//

#include "EngineGL.h"
#include <objects/Camera.h>
#include "LightGrid.h"
#include "objects/LightObject.h"
#include "objects/Projector.h"
#include "system/Logging.h"
#include "render/buffer/TextureBufferObject.h"
#include "render/shader/Uniform.h"
#include "render/debug/DebugDraw.h"
#include <functional>
#include "render/renderer/ICameraParamsProvider.h"

struct LightGridStruct {
  unsigned int offset;
  unsigned short pointLightCount;
  unsigned short spotLightCount;
  unsigned short projectorCount;
  unsigned short decalCount;
};

LightGrid::LightGrid(unsigned int cellSize) : _cellSize(cellSize) {

  // WebGL build doesn't allow to use texture buffer object, so conditionally fallback here
  // to use simple 2D texture as a buffer
#if ENGINE_USE_BUFFER_TEXTURE
  _lightGrid = std::make_shared<SwappableTextureBufferObject>(GL_RGB32UI, GL_DYNAMIC_DRAW);
  _lightIndex = std::make_shared<SwappableTextureBufferObject>(GL_R16UI, GL_DYNAMIC_DRAW);
#else
  _lightGrid = std::make_shared<SwappableTexture2DBuffer>(0, GL_RGB32UI);
  _lightIndex = std::make_shared<SwappableTexture2DBuffer>(4096, GL_R16UI);
#endif

  _lightGridBlock = UNIFORM_TEXTURE_BLOCKS.at(UniformName::LightGrid);
  _lightIndexBlock = UNIFORM_TEXTURE_BLOCKS.at(UniformName::LightIndices);
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
    cell.projectors.clear();
    cell.decals.clear();
  }
}

// Executes callback for every cell within projected edgePoints bounds
// edgePoints is vector because it has different length depending on the object
void LightGrid::_appendItem(const std::shared_ptr<ICameraParamsProvider> camera, const std::vector<vec3> &edgePoints,
                            std::function<void(LightGridCell *)> callback) {
  std::vector<vec3> projectedEdges(edgePoints.size());
  AABB bounds;

  for (int i = 0; i < edgePoints.size(); i++) {
    projectedEdges[i] = glm::project(edgePoints[i], camera->cameraViewMatrix(), camera->cameraProjectionMatrix(), camera->cameraViewport());
    if (i == 0) {
      bounds.min = projectedEdges[i];
      bounds.max = projectedEdges[i];
    } else {
      bounds.expand(projectedEdges[i]);
    }
  }

  auto startX = (int)round(floorf(fminf(fmaxf(bounds.min.x / _cellSize, 0), _cellsX - 1)));
  auto startY = (int)round(floorf(fminf(fmaxf(bounds.min.y / _cellSize, 0), _cellsY - 1)));
  auto endX = (int)round(floorf(fmaxf(fminf(bounds.max.x / _cellSize, _cellsX - 1), 0)));
  auto endY = (int)round(floorf(fmaxf(fminf(bounds.max.y / _cellSize, _cellsY - 1), 0)));

  if ((endX < 0) || (startX >= (int)_cellsX) || (endY < 0) || (startY >= (int)_cellsY)) {
    return; // light out of grid bounds
  }

  for (auto i = startX; i <= endX; i++ ) {
    for (auto j = startY; j <= endY; j++) {
      auto cell = _getCellByXY(i, j);
      callback(cell);
    }
  }
}

void LightGrid::appendLights(const std::vector<LightObjectPtr> &lights,
                             const std::shared_ptr<ICameraParamsProvider> camera) {
  _lightEdges.resize(4);

  for (auto &light : lights) {
    vec3 position = light->transform()->worldPosition();
    float radius = light->radius();

    _lightEdges[0] = position + camera->cameraLeft() * radius;
    _lightEdges[1] = position + camera->cameraRight() * radius;
    _lightEdges[2] = position + camera->cameraUp() * radius;
    _lightEdges[3] = position + camera->cameraDown() * radius;

    _appendItem(camera, _lightEdges, [&](LightGridCell *cell) {
      switch(light->type()) {
        case LightObjectType::Spot:
          cell->spotLights.push_back(light);
          break;
        case LightObjectType::Point:
          cell->pointLights.push_back(light);
          break;
      }
    });
  }
}

void LightGrid::appendProjectors(const std::vector<ProjectorPtr> &projectors,
                                 std::shared_ptr<ICameraParamsProvider> camera) {
  for (auto &projector : projectors) {
    projector->getEdgePoints(_lightEdges);

    _appendItem(camera, _lightEdges, [&](LightGridCell *cell) {
      switch(projector->type()) {
        case ProjectorType::Projector:
          cell->projectors.push_back(projector);
          break;
        case ProjectorType::Decal:
          cell->decals.push_back(projector);
          break;
      }
    });
  }
}

// Upload grid data into the GPU buffers
void LightGrid::upload() {
  _lightGrid->swap();
  _lightIndex->swap();

  auto gridBuffer = _lightGrid->current();
  auto indexBuffer = _lightIndex->current();

#if not ENGINE_USE_BUFFER_TEXTURE
  // texture 2D buffer stores data line-by-line so need to specify the line size
  gridBuffer->targetWidth(_cellsX);
#endif

  // Resize grid buffer to fit all the cell structs
  gridBuffer->resize((unsigned int)_cells.size() * sizeof(LightGridStruct));
  // Little bit unsafe but convenient way to directly modify data within the memory
  auto gridBufferPointer = (LightGridStruct *)gridBuffer->bufferPointer();

  unsigned int currentOffset = 0;
  for (int i = 0; i < _cells.size(); i++) {
    auto &cell = _cells[i];

    // Writing cell data
    // Referencing memory at the offset sizeof(LightGridStruct) * i
    gridBufferPointer[i].offset = currentOffset;
    gridBufferPointer[i].pointLightCount = (unsigned short)cell.pointLights.size();
    gridBufferPointer[i].spotLightCount = (unsigned short)cell.spotLights.size();
    gridBufferPointer[i].projectorCount = (unsigned short)cell.projectors.size();
    gridBufferPointer[i].decalCount = (unsigned short)cell.decals.size();

    // Writing indices
    // Count of light sources to put into the index data structure
    int indexDataSize = gridBufferPointer[i].pointLightCount +
                        gridBufferPointer[i].spotLightCount +
                        gridBufferPointer[i].projectorCount +
                        gridBufferPointer[i].decalCount;

    indexBuffer->resize((indexDataSize + currentOffset) * sizeof(unsigned short));
    // pointer should be obtained after resize() since resize may reallocate the data
    auto indexBufferPointer = (unsigned short *)indexBuffer->bufferPointer();

    // Indices for point lights
    for (int j = 0; j < gridBufferPointer[i].pointLightCount; j++) {
      indexBufferPointer[currentOffset + j] = (unsigned short)cell.pointLights[j]->index();
    }
    currentOffset += gridBufferPointer[i].pointLightCount;

    // Indices for spot lights
    for (int j = 0; j < gridBufferPointer[i].spotLightCount; j++) {
      indexBufferPointer[currentOffset + j] = (unsigned short)cell.spotLights[j]->index();
    }
    currentOffset += gridBufferPointer[i].spotLightCount;

    // Indices for projectors
    for (int j = 0; j < gridBufferPointer[i].projectorCount; j++) {
      indexBufferPointer[currentOffset + j] = (unsigned short)cell.projectors[j]->index();
    }
    currentOffset += gridBufferPointer[i].projectorCount;

    // Indices for decals
    for (int j = 0; j < gridBufferPointer[i].decalCount; j++) {
      indexBufferPointer[currentOffset + j] = (unsigned short)cell.decals[j]->index();
    }
    currentOffset += gridBufferPointer[i].decalCount;
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

void LightGrid::setDebugDraw(std::shared_ptr<DebugDraw> debugDraw) {
  _debugDraw = debugDraw;
}


