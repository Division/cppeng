//
// Created by Sidorenko Nikita on 4/11/18.
//

#ifndef CPPWRAPPER_VERTEXBUFFEROBJECT_H
#define CPPWRAPPER_VERTEXBUFFEROBJECT_H

#include "EngineGL.h"
#include <vector>
#include <memory>
#include "MemoryBuffer.h"

class VertexBufferObject : public MemoryBuffer {
public:
  VertexBufferObject(GLenum target, GLenum usage, unsigned int fixedSize = 0);
  ~VertexBufferObject() override;

  virtual void bind();
  virtual void unbind();
  void upload() override;
  GLuint vbo() { return _vbo; }

protected:
  unsigned int _bufferAllocatedSize = 0;
  GLuint _vbo = 0;
  GLenum _target;
  GLenum _usage;
  unsigned int _fixedSize;
  void _recreateBuffer();
};


typedef std::shared_ptr<VertexBufferObject> VertexBufferObjectPtr;

class SwappableVertexBufferObject : public SwappableBufferObject<VertexBufferObject> {
public:
  SwappableVertexBufferObject(GLenum target, GLenum usage, unsigned int fixedSize = 0, unsigned int count = 2)
      : SwappableBufferObject<VertexBufferObject>(count), _target(target), _usage(usage), _fixedSize(fixedSize) {
    _createBuffers();
  }

protected:
  std::shared_ptr<VertexBufferObject> createBuffer() override;

private:
  GLenum _target;
  GLenum _usage;
  unsigned int _fixedSize;
};

typedef std::shared_ptr<SwappableVertexBufferObject> SwappableVertexBufferObjectPtr;

#endif //CPPWRAPPER_VERTEXBUFFEROBJECT_H
