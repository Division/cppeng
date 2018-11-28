//
// Created by Sidorenko Nikita on 11/22/18.
//

#include "MultiVertexBufferObject.h"
#include "system/Logging.h"
#include "EngMath.h"
#include "EngineGL.h"

void MultiVertexBufferObject::map() {

  if (_buffers.size() <= _currentIndex) {
    ENGLog("multi vbo added");
    _buffers.emplace_back(std::make_shared<SwappableVertexBufferObject>(_target, _usage, _bufferSize));
  }

  _setCurrentBufferMapping(true);

  // Every time it's mapped next VBO is assigned
  // May be a subject to change by mapping to buffer ranges
  _isMapped = true;
}

void MultiVertexBufferObject::unmap() {
  _setCurrentBufferMapping(false);
  _currentIndex += 1;
  _currentOffset = 0;
  _isMapped = false;
}

MultiVBOAddress MultiVertexBufferObject::appendData(void *data, unsigned int size) {
  if (!_isMapped) {
    throw std::runtime_error("can't write to unmapped buffer");
  }

  unsigned int alignedSize = _alignment > 0 ? (unsigned int)(ceil((float)size / _alignment) * _alignment) : size;

  if (alignedSize > _bufferSize) {
    throw std::runtime_error("Can't write more data than buffer size");
  }

  // Current buffer is out of space?
  if (_currentOffset + alignedSize > _bufferSize) {
    _nextVBO();
  }
  auto resultOffset = _currentOffset; // address is the offset before size was added
  auto resultIndex = (unsigned int)_currentIndex;

  memcpy(&_mappedPointer[_currentOffset], data, size);
  _currentOffset += alignedSize; // include alignment

  return MultiVBOAddress{resultIndex, resultOffset};
}

// Should be called before data population
void MultiVertexBufferObject::swapBuffers() {
  _currentIndex = 0;
  _currentOffset = 0;

  for (auto &buffer : _buffers) {
    buffer->swap();
  }
}

// Must only be called while _isMapped
void MultiVertexBufferObject::_nextVBO() {
  _setCurrentBufferMapping(false);

  _currentIndex += 1;
  _currentOffset = 0;

  if (_buffers.size() <= _currentIndex) {
    ENGLog("multi vbo added");
    _buffers.emplace_back(std::make_shared<SwappableVertexBufferObject>(_target, _usage, _bufferSize));
  }

  _setCurrentBufferMapping(true);
}

void MultiVertexBufferObject::_setCurrentBufferMapping(bool mapped) {
  if (_currentIndex < 0 || _currentIndex >= this->_buffers.size()) {
    return;
  }

  auto &buffer = _buffers[_currentIndex];

#if USE_MEMORY_BUFFER_MAP
  if (mapped) {
    buffer->current()->bind();
    buffer->current()->resize(_bufferSize);
    _mappedPointer = buffer->current()->bufferPointer();
  } else {
    _mappedPointer = nullptr;
    buffer->current()->resize(_currentOffset);
    buffer->current()->upload();
    buffer->current()->unbind();
  };
#else
  if (mapped) {
    buffer->current()->bind();
    _mappedPointer = (char *)glMapBufferRange(_target, 0, _bufferSize, _access);
  } else {
    glUnmapBuffer(_target);
    buffer->current()->unbind();
  };
#endif

  engine::checkGLError();
}

unsigned int MultiVertexBufferObject::getVBO(unsigned int index) {
  return _buffers[index]->current()->vbo();
}
