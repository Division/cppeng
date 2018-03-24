//
// Created by Sidorenko Nikita on 2/15/18.
//

#include "Mesh.h"
#include <iostream>
#include "system/Logging.h"

const int JOINT_PER_VERTEX_MAX = 3;
const int JOINTS_MAX = 30;

const int VERTEX_SIZE = 3;
const int NORMAL_SIZE = 3;
const int TEXCOORD_SIZE = 2;
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
  _vbo = 0;
  _vao = 0;

  _hasIndices = false;
  _hasVertices = false;
  _hasNormals = false;
  _hasTBN = false;
  _hasTexCoord0 = false;
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

  _updateFaceCount();
}

void Mesh::setIndices(const std::vector<GLushort> &indices) {
  _indices = indices; // will be copied
  _hasIndices = true;
}


void Mesh::_deleteBuffer() {
  if (_vbo) {
    glDeleteBuffers(1, &_vbo);
    _vbo = 0;
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

  std::vector<float> bufferData;
  bufferData.resize((size_t)floor(_stride * _vertexCount));

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

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(GLfloat), &bufferData[0], _bufferUsage);

  if (_hasIndices) {
    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLfloat), &_indices[0], _bufferUsage);
  }

  glBindVertexArray(0);

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

  this->_prepareVAO();
}

void Mesh::_prepareVAO() {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  int off = this->vertexOffsetBytes();
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo());
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, this->componentCount(), GL_FLOAT, GL_FALSE, this->strideBytes(), BUFFER_OFFSET(off));


  glBindVertexArray(0);
}


int Mesh::_getStrideSize() {
  int result = 0;
  if (_hasVertices) result += VERTEX_SIZE;
  if (_hasNormals) result += NORMAL_SIZE;
  if (_hasTBN) result += NORMAL_SIZE * 2;
  if (_hasColors) result += COLOR_SIZE;
  if (_hasTexCoord0) result += TEXCOORD_SIZE;
  if (_hasWeights) result += JOINT_INDEX_SIZE + WEIGHT_SIZE;
  return result;
}

