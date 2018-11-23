//
// Created by Sidorenko Nikita on 11/22/18.
//

#include "MultiVertexBufferObject.h"
#include "system/Logging.h"

void MultiVertexBufferObject::map() {
  _isMapped = true;
  _setCurrentBufferMapping(true);

  // Create first buffer if required
  if (_buffers.empty()) {
    _currentIndex = -1;
    _nextVBO();
  }
}

void MultiVertexBufferObject::unmap() {
  _isMapped = false;
  _setCurrentBufferMapping(false);
}

MultiVBOAddress MultiVertexBufferObject::appendData(void *data, unsigned int size) {
  if (!_isMapped) {
    throw std::runtime_error("can't write to unmapped buffer");
  }

  if (size > _bufferSize) {
    throw std::runtime_error("Can't write more data than buffer size");
  }

  if (size > _bufferSize / 2.0f) {
    ENGLog("Ineffective use of MultiVertexBufferObject");
  }

  // Current buffer is out of space?
  if (_currentOffset + size > _bufferSize) {
    _nextVBO();
  }
  auto resultOffset = _currentOffset; // address is the offset before size added
  auto resultIndex = (unsigned int)_currentIndex;

  _currentOffset += size;
  memcpy(&_mappedPointer[_currentOffset], data, size);


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
    _buffers.emplace_back(std::make_shared<SwappableVertexBufferObject>(_target, _usage));
  }

  _setCurrentBufferMapping(true);
}

void MultiVertexBufferObject::_setCurrentBufferMapping(bool mapped) {
  if (_currentIndex < 0 || _currentIndex >= this->_buffers.size()) {
    return;
  }

  auto &buffer = _buffers[_currentIndex];

  if (mapped) {
    buffer->current()->bind();
    _mappedPointer = (char *)glMapBuffer(_target, _access);
  } else {
    glUnmapBuffer(_target);
    buffer->current()->unbind();
  };
}
