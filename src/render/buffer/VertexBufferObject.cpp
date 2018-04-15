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

void VertexBufferObject::resize(unsigned int size) {
  _size = size;
  if (size > _data.size()) {
    auto alignedSize = (unsigned int)(ceil((float)size / ALIGN_BYTES) * ALIGN_BYTES);
    _data.resize(alignedSize);
    setDirty();
  }
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

void VertexBufferObject::erase() {
  std::vector<char>().swap(_data);
  _size = 0;
}

int VertexBufferObject::writeData(void *data, unsigned int offset, unsigned int size) {
  if (offset + size > _size || size == 0) {
    throw std::invalid_argument("size bounds exceeded");
  }

  memcpy(&_data[offset], data, size);

  setDirty();

  return offset + size;
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

int VertexBufferObject::appendData(void *data, unsigned int size, unsigned int alignBytes) {
  auto offset = _size;
  resize(_size + size);
  writeData(data, offset, size);

  if (alignBytes > 0) {
    align(alignBytes);
  }

  return offset;
}

void VertexBufferObject::align(unsigned int bytes) {
  auto alignedSize = (unsigned int)(ceil((float)_size / bytes) * bytes);
  resize(alignedSize);
}

SwappableVertexBufferObject::SwappableVertexBufferObject(GLenum target, GLenum usage, int count) {
  _buffers.resize(count);
  for (auto &buffer : _buffers) {
    buffer = std::make_shared<VertexBufferObject>(target, usage);
  }
}

void SwappableVertexBufferObject::swap() {
  _currentIndex = (int)((_currentIndex + 1) % _buffers.size());
}
