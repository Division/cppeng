//
// Created by Sidorenko Nikita on 4/11/18.
//

#include "VertexBufferObject.h"
#include "EngMath.h"
#include <stdexcept>
#include "system/Logging.h"

const int ALIGN_BYTES = 4;

VertexBufferObject::VertexBufferObject(GLenum target, GLenum usage) : _target(target), _usage(usage) {
  glGenBuffers(1, &_vbo);
}

VertexBufferObject::~VertexBufferObject() {
  glDeleteBuffers(1, &_vbo);
}

inline void VertexBufferObject::bind() {
  glBindBuffer(_target, _vbo);
}


void VertexBufferObject::_recreateBuffer() {
  if (_size == 0) {
    return;
  }

  if (_target == GL_UNIFORM_BUFFER) {
    ENGLog("ubo resize %i ", _data.size());
  }

  _bufferAllocatedSize = _data.size();
  glBufferData(_target, _data.size(), nullptr, _usage);
}

void VertexBufferObject::upload() {
  if (!_dirty) {
    return;
  }

  bind();

  if (_bufferAllocatedSize < _data.size()) {
    _recreateBuffer();
  }

  glBufferSubData(_target, 0, _size, &_data[0]);
  _dirty = false;
}


std::shared_ptr<VertexBufferObject> SwappableVertexBufferObject::createBuffer() {
  return std::make_shared<VertexBufferObject>(_target, _usage);
}
