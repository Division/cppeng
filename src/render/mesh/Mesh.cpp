//
// Created by Sidorenko Nikita on 2/15/18.
//

#include "Mesh.h"
#include <iostream>
#include "system/Logging.h"
#include "render/shader/Shader.h"
#include "render/buffer/VertexBufferObject.h"

const int JOINT_PER_VERTEX_MAX = Mesh::JOINT_PER_VERTEX_MAX;
const int JOINTS_MAX = Mesh::JOINTS_MAX;

const int VERTEX_SIZE = 3;
const int NORMAL_SIZE = 3;
const int TEXCOORD_SIZE = 2;
const int CORNER_SIZE = 2;
const int JOINT_INDEX_SIZE = JOINT_PER_VERTEX_MAX;
const int WEIGHT_SIZE = JOINT_PER_VERTEX_MAX;
const int COLOR_SIZE = 4;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Mesh::Mesh(bool keepData, int componentCount, GLenum bufferUsage) :
    _keepData(keepData),
    _componentCount(componentCount),
    _bufferUsage(bufferUsage)
{
  _stride = 0;
  _faceCount = 0;
  _strideBytes = 0;
  _vertexCount = 0;
  _indexBuffer = 0;
  _vao = 0;

  _hasIndices = false;
  _hasVertices = false;
  _hasNormals = false;
  _hasTBN = false;
  _hasTexCoord0 = false;
  _hasCorners = false;
  _hasWeights = false;
  _hasColors = false;

  _vertexOffset = 0;
  _vertexOffsetBytes = 0;
  _normalOffset = 0;
  _normalOffsetBytes = 0;
  _tangentOffset = 0;
  _tangentOffsetBytes = 0;
  _bitangentOffset = 0;
  _bitangentOffsetBytes = 0;
  _texCoord0Offset = 0;
  _texCoord0OffsetBytes = 0;
  _jointIndexOffset = 0;
  _jointIndexOffsetBytes = 0;
  _weightOffset = 0;
  _weightOffsetBytes = 0;
  _colorOffset = 0;
  _colorOffsetBytes = 0;
}

Mesh::~Mesh() {
  _deleteBuffer();
}

// Setting mesh data

void Mesh::setVertices(const vec3 *vertices, int vertexCount) {
  _vertices.resize(vertexCount * 3);
  _vertexCount = vertexCount;
  memcpy(&_vertices[0], vertices, sizeof(vec3) * vertexCount);

  _hasVertices = true;

  _updateFaceCount();
}

void Mesh::setVertices(const float *vertexComponents, int vertexCount) {
  _vertices.resize(vertexCount * 3);
  _vertexCount = vertexCount;
  memcpy(&_vertices[0], vertexComponents, sizeof(vec3) * vertexCount);

  _hasVertices = true;

  _updateFaceCount();
}

void Mesh::setVertices(const std::vector<vec3> &vertices) {
  _vertices.resize(vertices.size() * 3);
  _vertexCount = (int)vertices.size();
  memcpy(&_vertices[0], &vertices[0], sizeof(vec3) * vertices.size());

  _hasVertices = true;

  _updateFaceCount();
}

void Mesh::setColors(const vec4 *colors, int colorCount) {
  _colors.resize(colorCount * 4);
  memcpy(&_colors[0], colors, sizeof(vec4) * colorCount);

  _hasColors = true;
}

void Mesh::setColors(const float *colorComponents, int colorCount) {
  _colors.resize(colorCount * 4);
  memcpy(&_colors[0], colorComponents, sizeof(vec4) * colorCount);

  _hasColors = true;
}

void Mesh::setColors(const std::vector<vec4> &colors) {
  _colors.resize(colors.size() * 4);
  memcpy(&_colors[0], &colors[0], sizeof(vec4) * colors.size());

  _hasColors = true;
}

void Mesh::setWeights(const std::vector<vec2> &weights) {

}

void Mesh::setWeights(const float *weightComponents, int count) {
  int size = count * JOINT_PER_VERTEX_MAX;
  _weights.resize(size);
  _jointIndices.resize(size);

  for (int i = 0; i < size; i++) {
    _jointIndices[i] = weightComponents[i * 2];
    _weights[i] = weightComponents[i * 2 + 1];
  }

  _hasWeights = true;
}

void Mesh::setTexCoord0(const float *components, int count) {
  _texCoord0.resize(count * 2);
  memcpy(&_texCoord0[0], components, sizeof(vec2) * count);

  _hasTexCoord0 = true;
}

void Mesh::setTexCoord0(const std::vector<vec2> &texcoords) {
  _texCoord0.resize(texcoords.size() * 2);
  memcpy(&_texCoord0[0], &texcoords[0], sizeof(vec2) * texcoords.size());

  _hasTexCoord0 = true;
}

void Mesh::setCorners(const float *components, int count) {
  _corners.resize(count * CORNER_SIZE);
  memcpy(&_corners[0], components, sizeof(vec2) * count);

  _hasCorners = true;
}

void Mesh::setCorners(const std::vector<vec2> &corners) {
  _corners.resize(corners.size() * CORNER_SIZE);
  memcpy(&_corners[0], &corners[0], sizeof(vec2) * corners.size());

  _hasCorners = true;
}

void Mesh::_updateFaceCount() {
  if (!_hasIndices) {
    _faceCount = (int)floor((float)_vertexCount/ (float)_componentCount);
  } else {
    _faceCount = (int)floor((float)_indices.size() / (float)_componentCount);
  }
}

void Mesh::setIndices(const GLushort *indices, int indexCount) {
  _indices.resize(indexCount);
  memcpy(&_indices[0], indices, sizeof(GLushort) * indexCount);
  _hasIndices = true;
  _updateFaceCount();
}

void Mesh::setIndices(const std::vector<GLushort> &indices) {
  _indices.assign(indices.begin(), indices.end()); // will be copied
  _hasIndices = true;
  _updateFaceCount();
}


void Mesh::_deleteBuffer() {
  if (_vao) {
    glDeleteVertexArrays(1, &_vao);
    _vao = 0;
  }
}

void Mesh::createBuffer() {
  _deleteBuffer();

  _stride = _getStrideSize();
  _strideBytes = _stride * 4;

  int currentOffset = 0;
  if (_hasVertices) {
    _vertexOffset = currentOffset;
    _vertexOffsetBytes = currentOffset * 4;
    currentOffset += VERTEX_SIZE;
    _vertexCount = (int)floor(_vertices.size() / 3.0f);
  }
  if (_hasNormals) {
    _normalOffset = currentOffset;
    _normalOffsetBytes = currentOffset * 4;
    currentOffset += NORMAL_SIZE;
  }
  if (_hasTBN) {
    _tangentOffset = currentOffset;
    _tangentOffsetBytes = currentOffset * 4;
    currentOffset += NORMAL_SIZE;

    _bitangentOffset = currentOffset;
    _bitangentOffsetBytes = currentOffset * 4;
    currentOffset += NORMAL_SIZE;
  }
  if (_hasTexCoord0) {
    _texCoord0Offset = currentOffset;
    _texCoord0OffsetBytes = currentOffset * 4;
    currentOffset += TEXCOORD_SIZE;
  }
  if (_hasCorners) {
    _cornerOffset = currentOffset;
    _cornerOffsetBytes = currentOffset * 4;
    currentOffset += CORNER_SIZE;
  }
  if (_hasWeights) {
    _weightOffset = currentOffset;
    _weightOffsetBytes = currentOffset * 4;
    currentOffset += WEIGHT_SIZE;

    _jointIndexOffset = currentOffset;
    _jointIndexOffsetBytes = currentOffset * 4;
    currentOffset += JOINT_INDEX_SIZE;
  }
  if (_hasColors) {
    _colorOffset = currentOffset;
    _colorOffsetBytes = currentOffset * 4;
    currentOffset += COLOR_SIZE;
  }

  // Unsafe but fast filling buffer data by working with pointer directly
  _vbo = std::make_shared<VertexBufferObject>(GL_ARRAY_BUFFER, _bufferUsage);
  _vbo->resize(_strideBytes * _vertexCount);
  float *bufferData = (float *)_vbo->bufferPointer();

  for (int i = 0; i < _vertexCount; i++) {
    int offset = i * _stride;
    if (_hasVertices) {
      currentOffset = offset + _vertexOffset;
      bufferData[currentOffset] = _vertices[i * 3];
      bufferData[currentOffset + 1] = _vertices[i * 3 + 1];
      bufferData[currentOffset + 2] = _vertices[i * 3 + 2];
    }

    if (_hasNormals) {
      currentOffset = offset + _normalOffset;
      bufferData[currentOffset] = _normals[i * 3];
      bufferData[currentOffset + 1] = _normals[i * 3 + 1];
      bufferData[currentOffset + 2] = _normals[i * 3 + 2];
    }

    if (_hasTBN) {
      currentOffset = offset + _tangentOffset;
      bufferData[currentOffset] = _tangents[i * 3];
      bufferData[currentOffset + 1] = _tangents[i * 3 + 1];
      bufferData[currentOffset + 2] = _tangents[i * 3 + 2];

      currentOffset = offset + _bitangentOffset;
      bufferData[currentOffset] = _bitangents[i * 3];
      bufferData[currentOffset + 1] = _bitangents[i * 3 + 1];
      bufferData[currentOffset + 2] = _bitangents[i * 3 + 2];
    }

    if (_hasTexCoord0) {
      currentOffset = offset + _texCoord0Offset;
      bufferData[currentOffset] = _texCoord0[i * 2];
      bufferData[currentOffset + 1] = _texCoord0[i * 2 + 1];
    }

    if (_hasCorners) {
      currentOffset = offset + _cornerOffset;
      bufferData[currentOffset] = _corners[i * 2];
      bufferData[currentOffset + 1] = _corners[i * 2 + 1];
    }

    if (_hasWeights) {
      currentOffset = offset + _jointIndexOffset;
      for (int j = 0; j < JOINT_INDEX_SIZE; j++) {
        bufferData[currentOffset + j] = _jointIndices[i * JOINT_INDEX_SIZE + j];
      }

      currentOffset = offset + _weightOffset;
      for (int j = 0; j < WEIGHT_SIZE; j++) {
        bufferData[currentOffset + j] = _weights[i * WEIGHT_SIZE + j];
      }
    }

    if (_hasColors) {
      currentOffset = offset + _colorOffset;
      bufferData[currentOffset] = _colors[i * 4];
      bufferData[currentOffset + 1] = _colors[i * 4 + 1];
      bufferData[currentOffset + 2] = _colors[i * 4 + 2];
      bufferData[currentOffset + 3] = _colors[i * 4 + 3];
    }
  }

  _vbo->upload();
  _vbo->erase();

  if (_hasIndices) {
    _indexBuffer = std::make_shared<VertexBufferObject>(GL_ELEMENT_ARRAY_BUFFER, _bufferUsage);
    unsigned int indexSize = (unsigned int)_indices.size() * (unsigned int)sizeof(GLushort);
    _indexBuffer->resize(indexSize);
    _indexBuffer->writeData((void *)&_indices[0], 0, indexSize);
    _indexBuffer->upload();
    _indexBuffer->erase();
  }

  if (!_vao) {
    this->_prepareVAO();
  }

  _calculateAABB();

  // Free data arrays
  if (!_keepData) {
    std::vector<GLfloat>().swap(_vertices);
    std::vector<GLfloat>().swap(_normals);
    std::vector<GLfloat>().swap(_tangents);
    std::vector<GLfloat>().swap(_bitangents);
    std::vector<GLfloat>().swap(_texCoord0);
    std::vector<GLfloat>().swap(_weights);
    std::vector<GLfloat>().swap(_jointIndices);
    std::vector<GLfloat>().swap(_colors);
    std::vector<GLushort>().swap(_indices);
  }
}

void Mesh::_prepareVAO() {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  _vbo->bind();

  if (_hasIndices) {
    _indexBuffer->bind();
  }

  void *offset;
  GLuint attribIndex = 0;
  offset = BUFFER_OFFSET(this->vertexOffsetBytes());

  attribIndex = (GLuint)ShaderAttrib::Position;
  glEnableVertexAttribArray(attribIndex);
  glVertexAttribPointer(attribIndex, VERTEX_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);

  if (_hasColors) {
    attribIndex = (GLuint)ShaderAttrib::VertexColor;
    offset = BUFFER_OFFSET(this->colorOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, COLOR_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);
  }


  if (_hasNormals) {
    attribIndex = (GLuint)ShaderAttrib::Normal;
    offset = BUFFER_OFFSET(this->normalOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, VERTEX_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);
  }

  if (_hasTBN) {
    attribIndex = (GLuint)ShaderAttrib::Tangent;
    offset = BUFFER_OFFSET(this->tangentOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, VERTEX_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);

    attribIndex = (GLuint)ShaderAttrib::Bitangent;
    offset = BUFFER_OFFSET(this->bitangentOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, VERTEX_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);
  }

  if (_hasTexCoord0) {
    attribIndex = (GLuint)ShaderAttrib::TexCoord0;
    offset = BUFFER_OFFSET(this->texCoordOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);
  }

  if (_hasCorners) {
    attribIndex = (GLuint)ShaderAttrib::Corner;
    offset = BUFFER_OFFSET(this->cornerOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, CORNER_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);
  }

  if (_hasWeights) {
    attribIndex = (GLuint)ShaderAttrib::JointWeights;
    offset = BUFFER_OFFSET(this->weightOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, WEIGHT_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);

    attribIndex = (GLuint)ShaderAttrib::JointIndices;
    offset = BUFFER_OFFSET(this->jointIndexOffsetBytes());
    glEnableVertexAttribArray(attribIndex);
    glVertexAttribPointer(attribIndex, JOINT_INDEX_SIZE, GL_FLOAT, GL_FALSE, this->strideBytes(), offset);
  }

  glBindVertexArray(0);
}


int Mesh::_getStrideSize() {
  int result = 0;
  if (_hasVertices) result += VERTEX_SIZE;
  if (_hasNormals) result += NORMAL_SIZE;
  if (_hasTBN) result += NORMAL_SIZE * 2;
  if (_hasColors) result += COLOR_SIZE;
  if (_hasTexCoord0) result += TEXCOORD_SIZE;
  if (_hasCorners) result += CORNER_SIZE;
  if (_hasWeights) result += JOINT_INDEX_SIZE + WEIGHT_SIZE;
  return result;
}

void Mesh::calculateNormals() {
  _normals.resize(_vertices.size());
  memset(&_normals[0], 0, _normals.size() * sizeof(float));

  for (int i = 0; i < _faceCount; i++) {
    int faceOffset = i * 3;
    int indexA = _hasIndices ? _indices[faceOffset] * 3 : faceOffset * 3;
    int indexB = _hasIndices ? _indices[faceOffset + 1] * 3 : faceOffset * 3 + 3;
    int indexC = _hasIndices ? _indices[faceOffset + 2] * 3 : faceOffset * 3 + 6;

    vec3 a(_vertices[indexA], _vertices[indexA + 1], _vertices[indexA + 2]);
    vec3 b(_vertices[indexB], _vertices[indexB + 1], _vertices[indexB + 2]);
    vec3 c(_vertices[indexC], _vertices[indexC + 1], _vertices[indexC + 2]);
    b = b - a;
    c = c - a;

    vec3 normal = glm::cross(b, c);

    for (int j = 0; j < 3; j++) {
      _normals[indexA + j] += normal[j];
      _normals[indexB + j] += normal[j];
      _normals[indexC + j] += normal[j];
    }
  }

  for (int i = 0; i < _vertexCount; i++) {
    auto *normal = (vec3 *)&_normals[i * 3];
    *normal = glm::normalize(*normal);
  }

  _hasNormals = true;
}

void Mesh::calculateTBN() {
  if (!this->_hasTexCoord0) {
    throw std::runtime_error("Can't calculate tangent space without TexCoord0");
  }

  _tangents.resize(_vertices.size());
  _bitangents.resize(_vertices.size());
  memset(&_tangents[0], 0, _tangents.size() * sizeof(float));
  memset(&_bitangents[0], 0, _bitangents.size() * sizeof(float));

  for (int i = 0; i < _faceCount; i++) {
    int faceOffset = i * 3;
    int indexA = _hasIndices ? _indices[faceOffset] * 3 : faceOffset * 3;
    int indexB = _hasIndices ? _indices[faceOffset + 1] * 3 : faceOffset * 3 + 3;
    int indexC = _hasIndices ? _indices[faceOffset + 2] * 3 : faceOffset * 3 + 6;

    int indexUVA = _hasIndices ? _indices[faceOffset] * 2 : faceOffset * 2;
    int indexUVB = _hasIndices ? _indices[faceOffset + 1] * 2 : faceOffset * 2 + 2;
    int indexUVC = _hasIndices ? _indices[faceOffset + 2] * 2 : faceOffset * 2 + 4;

    vec3 a(_vertices[indexA], _vertices[indexA + 1], _vertices[indexA + 2]);
    vec3 b(_vertices[indexB], _vertices[indexB + 1], _vertices[indexB + 2]);
    vec3 c(_vertices[indexC], _vertices[indexC + 1], _vertices[indexC + 2]);

    vec2 uvA(_texCoord0[indexUVA], _texCoord0[indexUVA + 1]);
    vec2 uvB(_texCoord0[indexUVB], _texCoord0[indexUVB + 1]);
    vec2 uvC(_texCoord0[indexUVC], _texCoord0[indexUVC + 1]);

    vec3 deltaPos1 = b - a;
    vec3 deltaPos2 = c - a;

    vec2 deltaUV1 = uvB - uvA;
    vec2 deltaUV2 = uvC - uvA;

    float r = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);
    a = deltaPos1 * (deltaUV2[1] * r);
    b = deltaPos2 * (deltaUV1[1] * r);
    vec3 tangent = a - b;

    a = deltaPos2 * (deltaUV1[0] * r);
    b = deltaPos1 * (deltaUV2[0] * r);
    vec3 bitangent = a - b;

    for (int j = 0; j < 3; j++) {
      _tangents[indexA + j] += tangent[j];
      _tangents[indexB + j] += tangent[j];
      _tangents[indexC + j] += tangent[j];
      _bitangents[indexA + j] += bitangent[j];
      _bitangents[indexB + j] += bitangent[j];
      _bitangents[indexC + j] += bitangent[j];
    }
  }

  for (int i = 0; i < _vertexCount; i++) {
    vec3 *a = (vec3 *)&_tangents[i * 3];
    vec3 *b = (vec3 *)&_bitangents[i * 3];
    vec3 *c = (vec3 *)&_normals[i * 3];

    // Orthonormalize matrix. Since it's almost orthonormal we can just correct tangent a little.
    // t = normalize(t - n * dot(n, t));
    *a = *a - *c * glm::dotf(*c, *a);

    // Check the tangent direction
    if (glm::dotf(glm::cross(*c, *a), *b) < 0) {
      *a = -*a; // invert tangent
    }

    *a = -glm::normalize(*a);
    *b = -glm::normalize(*b); // have no idea why it should be inverted to look visually correct
  }

  _hasTBN = true;
}

void Mesh::_calculateAABB() {
  if (_vertexCount == 0) {
    _aabb.min = vec3(0);
    _aabb.max = vec3(0);
    return;
  }

  vec3 *vertices = (vec3 *)&_vertices[0];
  _aabb.min = vertices[0];
  _aabb.max = vertices[0];
  for (int i = 1; i < _vertexCount; i++) {
    _aabb.min = glm::min(_aabb.min, vertices[i]);
    _aabb.max = glm::max(_aabb.max, vertices[i]);
  }
}
