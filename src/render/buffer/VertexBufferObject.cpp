//
// Created by Sidorenko Nikita on 4/11/18.
//

#include "VertexBufferObject.h"
#include "EngMath.h"
#include <stdexcept>
#include "system/Logging.h"

const int ALIGN_BYTES = 4;

VertexBufferObject::VertexBufferObject(GLenum target, GLenum usage, unsigned int fixedSize)
    : _target(target), _usage(usage), _fixedSize(fixedSize) {
  glGenBuffers(1, &_vbo);
  bind();
  _recreateBuffer();
}

VertexBufferObject::~VertexBufferObject() {
  glDeleteBuffers(1, &_vbo);
}

inline void VertexBufferObject::bind() {
  glBindBuffer(_target, _vbo);
}

inline void VertexBufferObject::unbind() {
  glBindBuffer(_target, 0);
}

void VertexBufferObject::_recreateBuffer() {
  auto targetSize = _data.size();

  if (_fixedSize > 0) {
    targetSize = _fixedSize;
    if (_size > _fixedSize) {
      throw std::runtime_error("VBO fixedSize exceeded");
    }
  } else if (_size == 0) {
    return;
  }

//  if (_target == GL_UNIFORM_BUFFER) {
//    ENGLog("ubo resize %i ", targetSize);
//  }

  _bufferAllocatedSize = (unsigned)targetSize;
  glBufferData(_target, targetSize, nullptr, _usage);
}

void VertexBufferObject::upload() {
  if (!_dirty || !_size) {
    return;
  }

  bind();

  if (_bufferAllocatedSize < _data.size()) {
    _recreateBuffer();
  }

  glBufferSubData(_target, 0, _size, &_data[0]);
  engine::checkGLError();

  _dirty = false;
  this->resize(0);
}

std::shared_ptr<VertexBufferObject> SwappableVertexBufferObject::createBuffer() {
  return std::make_shared<VertexBufferObject>(_target, _usage, _fixedSize);
}
