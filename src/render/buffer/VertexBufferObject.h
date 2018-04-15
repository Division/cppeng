//
// Created by Sidorenko Nikita on 4/11/18.
//

#ifndef CPPWRAPPER_VERTEXBUFFEROBJECT_H
#define CPPWRAPPER_VERTEXBUFFEROBJECT_H

#include "EngineGL.h"
#include <vector>
#include <memory>

class VertexBufferObject {
public:
  explicit VertexBufferObject(GLenum target, GLenum usage);
  ~VertexBufferObject();

  void bind();
  void resize(unsigned int size);
  int writeData(void *data, unsigned int offset, unsigned int size);
  int appendData(void *data, unsigned int size, unsigned int alignBytes = 0);
  void align(unsigned int bytes);
  void upload();
  void erase();
  void setDirty() { _dirty = true; };
  char *bufferPointer() { return &_data[0]; };
  GLuint vbo() { return _vbo; }

private:
  unsigned int _size = 0;
  unsigned int _bufferAllocatedSize = 0;
  bool _dirty;
  GLenum _target;
  GLenum _usage;
  GLuint _vbo = 0;
  std::vector<char> _data;

  void _recreateBuffer();
};

typedef std::shared_ptr<VertexBufferObject> VertexBufferObjectPtr;

class SwappableVertexBufferObject {
public:
  SwappableVertexBufferObject(GLenum target, GLenum usage, int count = 2);
  void swap();
  VertexBufferObjectPtr current() { return _buffers[_currentIndex]; }
private:
  int _currentIndex = 0;
  std::vector<VertexBufferObjectPtr> _buffers;
};

typedef std::unique_ptr<SwappableVertexBufferObject> SwappableVertexBufferObjectUniquePtr;

#endif //CPPWRAPPER_VERTEXBUFFEROBJECT_H
