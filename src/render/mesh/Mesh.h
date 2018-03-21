//
// Created by Sidorenko Nikita on 2/15/18.
//

#ifndef CPPWRAPPER_MESH_H
#define CPPWRAPPER_MESH_H

#include "SDL_opengl.h"
#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class Mesh {
public:
  explicit Mesh(bool keepData = false, int componentCount = 3, GLenum bufferUsage = GL_STATIC_DRAW);
  virtual ~Mesh();

  void setVertices(const vec3 *vertices, int vertexCount);
  void setVertices(const float *vertexComponents, int vertexCount);
  void setVertices(const std::vector<vec3> &vertices);

  void setIndices(const GLushort *indices, int indexCount);
  void setIndices(const std::vector<GLushort> &indices);

  void createBuffer();

  // Properties

  GLuint vbo() const { return _vbo; }

  int componentCount() const { return _componentCount; }
  int strideBytes() const { return _strideBytes; };
  int faceCount() const {return _faceCount; }

  bool hasVertices() const { return _hasVertices; }
  bool hasIndices() const { return _hasIndices; }
  bool hasNormals() const { return _hasNormals; }
  bool hasTBN() const { return _hasTBN; }
  bool hasTexCoord0() const { return _hasTexCoord0; }
  bool hasWeights() const { return _hasWeights; }
  bool hasColors() const { return _hasColors; }

  int vertexOffsetBytes() const { return _vertexOffsetBytes; };
  int normalOffsetBytes() const { return _normalOffsetBytes; };
  int tangentOffsetBytes() const { return _tangentOffsetBytes; };
  int bitangentOffsetBytes() const { return _bitangentOffsetBytes; };
  int texCoordOffsetBytes() const { return _texCoord0OffsetBytes; };
  int jointIndexOffsetBytes() const { return _jointIndexOffsetBytes; };
  int weightOffsetBytes() const { return _weightOffsetBytes; };
  int colorOffsetBytes() const { return _colorOffsetBytes; };

private:
  int _getStrideSize();
  void _deleteBuffer();

private:
  bool _keepData;
  int _componentCount;
  int _faceCount;
  int _stride;
  int _strideBytes;
  int _vertexCount;
  GLenum _bufferUsage;
  GLuint _indexBuffer;
  GLuint _vbo;

  // Attrib flags
  bool _hasIndices;
  bool _hasVertices;
  bool _hasNormals;
  bool _hasTBN;
  bool _hasTexCoord0;
  bool _hasWeights;
  bool _hasColors;

  // Offsets
  int _vertexOffset;
  int _vertexOffsetBytes;
  int _normalOffset;
  int _normalOffsetBytes;
  int _tangentOffset;
  int _tangentOffsetBytes;
  int _bitangentOffset;
  int _bitangentOffsetBytes;
  int _texCoord0Offset;
  int _texCoord0OffsetBytes;
  int _jointIndexOffset;
  int _jointIndexOffsetBytes;
  int _weightOffset;
  int _weightOffsetBytes;
  int _colorOffset;
  int _colorOffsetBytes;

  // Attrib data
  std::vector<GLushort> _indices;
  std::vector<GLfloat> _vertices;
  std::vector<GLfloat> _normals;
  std::vector<GLfloat> _tangents;
  std::vector<GLfloat> _bitangents;
  std::vector<GLfloat> _texCoord0;
  std::vector<GLfloat> _weights;
  std::vector<GLfloat> _jointIndices;
  std::vector<GLfloat> _colors;

  void _updateFaceCount();
};


#endif //CPPWRAPPER_MESH_H
