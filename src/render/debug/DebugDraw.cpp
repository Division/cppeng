//
// Created by Sidorenko Nikita on 2/15/18.
//

#include "DebugDraw.h"
#include "EngineGL.h"
#include "render/mesh/Mesh.h"
#include "EngMath.h"
#include "utils/MeshGeneration.h"
#include "EngineMain.h"
#include "render/shader/ShaderGenerator.h"

DebugDraw::DebugDraw() {
  _quadMesh = std::make_shared<Mesh>(true, 3, GL_STATIC_DRAW);
  MeshGeneration::generateQuad(_quadMesh, vec2(1, 1));
  _quadMesh->createBuffer();

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  _textureShader = getEngine()->getShaderWithCaps(caps);
  _textureShader->addUniformBlock(UniformBlockName::ObjectParams);
  _textureShader->addUniformBlock(UniformBlockName::Camera);

  auto generator = getEngine()->shaderGenerator();
  generator->addTemplate("debug/depth.glsl");
  _depthMapShader = generator->getShaderWithCaps(caps, "debug/depth.glsl");
  _depthMapShader->addUniform(UniformName::Texture0);
  _depthMapShader->addUniform(UniformName::NearFar);
  _depthMapShader->addUniformBlock(UniformBlockName::ObjectParams);
  _depthMapShader->addUniformBlock(UniformBlockName::Camera);

  _lineMeshes[0] = std::make_shared<Mesh>(true, 2, GL_DYNAMIC_DRAW);
  _lineMeshes[1] = std::make_shared<Mesh>(true, 2, GL_DYNAMIC_DRAW);
  _pointMeshes[0] = std::make_shared<Mesh>(true, 1, GL_DYNAMIC_DRAW);
  _pointMeshes[1] = std::make_shared<Mesh>(true, 1, GL_DYNAMIC_DRAW);
}

void DebugDraw::drawLine(const vec3 &p1, const vec3 &p2, const vec4 &color) {
  _lines.push_back(p1);
  _lines.push_back(p2);
  _lineColors.push_back(color);
  _lineColors.push_back(color);
}

void DebugDraw::drawPoint(const vec3 &p, const vec3 &color, float size) {
  _points.push_back(p);
  _pointColors.push_back(vec4(color, size));
}

void DebugDraw::drawFrustum(mat4 viewProjection, vec4 color) {
  auto inv = glm::inverse(viewProjection);
  vec4 quad1[4] = {
      inv * vec4(1, -1, -1, 1),
      inv * vec4(-1, -1, -1, 1),
      inv * vec4(-1, 1, -1, 1),
      inv * vec4(1, 1, -1, 1),
  };

  vec4 quad2[4] = {
      inv * vec4(1, -1, 1, 1),
      inv * vec4(-1, -1, 1, 1),
      inv * vec4(-1, 1, 1, 1),
      inv * vec4(1, 1, 1, 1),
  };

  for (int i = 0; i < 4; i++) {
    quad1[i] /= quad1[i].w;
    quad2[i] /= quad2[i].w;
  }

  for (int i = 0; i < 4; i++) {
    drawPoint(quad1[i], vec3(color), 10);
    drawLine(quad1[i], quad1[(i + 1) % 4], color);
    drawLine(quad2[i], quad2[(i + 1) % 4], color);
    drawLine(quad1[i], quad2[i], color);
  }
}

void DebugDraw::drawAABB(const vec3 &min, const  vec3 &max, const vec4 &color) {
  vec3 size = max - min;
  const vec3 vertices[] = {
      min,
      min + size * vec3(0, 0, 1),
      min + size * vec3(1, 0, 1),
      min + size * vec3(1, 0, 0),
      max - size * vec3(1, 0, 1),
      max - size * vec3(1, 0, 0),
      max,
      max - size * vec3(0, 0, 1),
  };

  for (int i = 0; i < 4; i++) {
    drawLine(vertices[i], vertices[(i + 1) % 4], color);
    drawLine(vertices[i + 4], vertices[(i + 1) % 4 + 4], color);
    drawLine(vertices[i], vertices[i + 4], color);
  }
}

void DebugDraw::drawAABB(const AABB &bounds, const vec4 &color) {
  drawAABB(bounds.min, bounds.max, color);
}

void DebugDraw::drawOBB(const OBB &bounds, const vec4 &color) {
  vec3 size = bounds.max - bounds.min;
  vec3 min = bounds.min;
  vec3 max = bounds.max;
  vec3 vertices[] = {
      min,
      min + size * vec3(0, 0, 1),
      min + size * vec3(1, 0, 1),
      min + size * vec3(1, 0, 0),
      max - size * vec3(1, 0, 1),
      max - size * vec3(1, 0, 0),
      max,
      max - size * vec3(0, 0, 1),
  };

  for (vec3 &v : vertices) {
    v = vec3(bounds.matrix * vec4(v, 1));
  }

  for (int i = 0; i < 4; i++) {
    drawLine(vertices[i], vertices[(i + 1) % 4], color);
    drawLine(vertices[i + 4], vertices[(i + 1) % 4 + 4], color);
    drawLine(vertices[i], vertices[i + 4], color);
  }
}

void DebugDraw::render(std::shared_ptr<IRenderer> &renderer) {
  if (!_material) { // needed to move material creation out from constructor because renderer isn't fully created yet
    _material = std::make_shared<MaterialDebug>();
  }

  _currentMeshIndex = (_currentMeshIndex + 1) % 2;
  auto lineMesh = _lineMeshes[_currentMeshIndex];
  auto pointMesh = _pointMeshes[_currentMeshIndex];

  if (!_lines.empty()) {
    lineMesh->setVertices(_lines);
    lineMesh->setColors(_lineColors);
    lineMesh->createBuffer();
    RenderOperation lineROP;
    lineROP.objectParams = &_renderParams;
    lineROP.mesh = lineMesh;
    lineROP.material = _material;
    lineROP.modelMatrix = mat4();
    lineROP.debugInfo = "debug lines";
    lineROP.mode = GL_LINES;
    renderer->addRenderOperation(lineROP, RenderQueue::Debug);
  }

  if (!_points.empty()) {
    pointMesh->setVertices(_points);
    pointMesh->setColors(_pointColors);
    pointMesh->createBuffer();
    RenderOperation pointROP;
    pointROP.objectParams = &_renderParams;
    pointROP.mesh = pointMesh;
    pointROP.material = _material;
    pointROP.modelMatrix = mat4();
    pointROP.debugInfo = "debug points";
    pointROP.mode = GL_POINTS;
    renderer->addRenderOperation(pointROP, RenderQueue::Debug);
  }

  for (int i = 0; i < _imageCount; i++) {
    auto &image = _images[i];
    RenderOperation imageROP;
    imageROP.objectParams = &image.renderParams;
    imageROP.mesh = _quadMesh;
    imageROP.material = image.material;
    imageROP.modelMatrix = glm::translate(mat4(), vec3(image.bounds.x, image.bounds.y, 0));
    imageROP.modelMatrix = glm::scale(imageROP.modelMatrix, vec3(image.bounds.z, image.bounds.w, 1));
    imageROP.debugInfo = "debug texture";
    imageROP.mode = GL_TRIANGLES;
    renderer->addRenderOperation(imageROP, RenderQueue::UI);
  }

  for (int i = 0; i < _depthMapImageCount; i++) {
    auto &image = _depthMapImages[i];
    RenderOperation imageROP;
    imageROP.objectParams = &image.renderParams;
    imageROP.mesh = _quadMesh;
    imageROP.material = image.material;
    imageROP.modelMatrix = glm::translate(mat4(), vec3(image.bounds.x, image.bounds.y, 0));
    imageROP.modelMatrix = glm::scale(imageROP.modelMatrix, vec3(image.bounds.z, image.bounds.w, 1));
    imageROP.debugInfo = "debug depth map";
    imageROP.mode = GL_TRIANGLES;
    renderer->addRenderOperation(imageROP, RenderQueue::UI);
  }

  _imageCount = 0;
  _depthMapImageCount = 0;
  _lines.resize(0);
  _points.resize(0);
  _lineColors.resize(0);
  _pointColors.resize(0);
}

void DebugDraw::drawImage(TexturePtr texture, vec4 bounds) {
  _imageCount += 1;

  if (_imageCount > _images.size()) {
    _images.emplace_back(Image(bounds));
  }

  _images[_imageCount - 1].material->texture(texture);
  _images[_imageCount - 1].material->setShader(_textureShader);
}

void DebugDraw::drawDepthImage(TexturePtr texture, vec4 bounds, vec2 nearFar) {
  _depthMapImageCount += 1;

  if (_depthMapImageCount> _depthMapImages.size()) {
    _depthMapImages.emplace_back(DepthMapImage(bounds));
  }

  _depthMapImages[_depthMapImageCount - 1].material->texture(texture);
  _depthMapImages[_depthMapImageCount - 1].material->setNearFar(nearFar);
  _depthMapImages[_depthMapImageCount - 1].material->setShader(_depthMapShader);
}

DebugDraw::DebugTextureMaterial::DebugTextureMaterial() {
  _texture0Binding = _addTextureBinding(UniformName::Texture0);
}

DebugDraw::DebugDepthMapMaterial::DebugDepthMapMaterial() {
  _texture0Binding = _addTextureBinding(UniformName::Texture0);
  _nearFarBinding = _addVec2Binding(UniformName::NearFar);
}
