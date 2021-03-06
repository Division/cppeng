//
// Created by Sidorenko Nikita on 2/15/18.
//

#ifndef CPPWRAPPER_MESH_H
#define CPPWRAPPER_MESH_H

#include "EngineGL.h"
#include "EngMath.h"
#include <vector>
#include <memory>

using namespace glm;

extern const int JOINT_PER_VERTEX_MAX;

class VertexBufferObject;

class Mesh {
public:
  explicit Mesh(bool keepData = true, int componentCount = 3, GLenum bufferUsage = GL_STATIC_DRAW);
  virtual ~Mesh();

  static const int JOINTS_MAX = 70;
  static const int JOINT_PER_VERTEX_MAX = 3;

  void setVertices(const vec3 *vertices, int vertexCount);
  void setVertices(const float *vertexComponents, int vertexCount);
  void setVertices(const std::vector<vec3> &vertices);

  void setColors(const vec4 *colors, int colorCount);
  void setColors(const float *colorComponents, int colorCount);
  void setColors(const std::vector<vec4> &colors);

  void setWeights(const std::vector<vec2> &weights);
  void setWeights(const float *weightComponents, int count);

  void setTexCoord0(const std::vector<vec2> &texcoords);
  void setTexCoord0(const float *texcoordComponents, int count);

  void setCorners(const std::vector<vec2> &corners);
  void setCorners(const float *components, int count);

  void setIndices(const GLushort *indices, int indexCount);
  void setIndices(const std::vector<GLushort> &indices);

  void calculateNormals();
  void calculateTBN();

  void createBuffer();

  AABB aabb() const { return _aabb; }

  // Properties

  GLuint vao() const { return _vao; }
  std::shared_ptr<VertexBufferObject> vbo() { return _vbo; }

  int componentCount() const { return _componentCount; } // Number of vertices to complete the primitive (3 for triangle)
  int strideBytes() const { return _strideBytes; };
  int faceCount() const {return _faceCount; }
  int indexCount() const { return _faceCount * _componentCount; }
  int vertexCount() const { return _vertexCount; }
  vec3 getVertex(int index) const {
    if (!_hasVertices || !_keepData) { return vec3(); }
    else { return vec3(_vertices[index * 3], _vertices[index * 3 + 1], _vertices[index * 3 + 2]); };
  }
  vec4 getWeights(int index) const {
    if (!_hasWeights|| !_keepData) { return vec4(); }
    else { return vec4(_weights[index * JOINT_PER_VERTEX_MAX], _weights[index * JOINT_PER_VERTEX_MAX + 1], _weights[index * JOINT_PER_VERTEX_MAX + 2], 0); };
  }
  ivec4 getJointIndices(int index) const {
    if (!_hasWeights|| !_keepData) { return ivec4(); }
    else { return ivec4(lround(_jointIndices[index * JOINT_PER_VERTEX_MAX]),
                        lround(_jointIndices[index * JOINT_PER_VERTEX_MAX + 1]),
                        lround(_jointIndices[index * JOINT_PER_VERTEX_MAX + 2]),
                        0);
    };
  }

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
  int cornerOffsetBytes() const { return _cornerOffsetBytes; };
  int jointIndexOffsetBytes() const { return _jointIndexOffsetBytes; };
  int weightOffsetBytes() const { return _weightOffsetBytes; };
  int colorOffsetBytes() const { return _colorOffsetBytes; };

private:
  int _getStrideSize();
  void _deleteBuffer();
  void _updateFaceCount();
  void _prepareVAO();
  void _calculateAABB();

private:
  AABB _aabb;
  std::shared_ptr<VertexBufferObject> _vbo;
  std::shared_ptr<VertexBufferObject> _indexBuffer;

  bool _keepData;
  int _componentCount;
  int _faceCount;
  int _stride;
  int _strideBytes;
  int _vertexCount;
  GLenum _bufferUsage;
  GLuint _vao;

  // Attrib flags
  bool _hasIndices;
  bool _hasVertices;
  bool _hasNormals;
  bool _hasTBN;
  bool _hasTexCoord0;
  bool _hasCorners;
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
  int _cornerOffset;
  int _cornerOffsetBytes;
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
  std::vector<GLfloat> _corners;
  std::vector<GLfloat> _weights;
  std::vector<GLfloat> _jointIndices;
  std::vector<GLfloat> _colors;
};

typedef std::shared_ptr<Mesh> MeshPtr;


#endif //CPPWRAPPER_MESH_H
