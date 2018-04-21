//
// Created by Sidorenko Nikita on 4/20/18.
//

#ifndef CPPWRAPPER_MEMORYBUFFER_H
#define CPPWRAPPER_MEMORYBUFFER_H

#include "EngineGL.h"
#include <vector>
#include <memory>

class MemoryBuffer {
public:
  MemoryBuffer() = default;
  virtual ~MemoryBuffer() = default;

  virtual void upload();

  void resize(unsigned int size);
  int writeData(void *data, unsigned int offset, unsigned int size);
  int appendData(void *data, unsigned int size, unsigned int alignBytes = 0);
  void align(unsigned int bytes);
  void erase();
  void setDirty() { _dirty = true; };
  char *bufferPointer() { return &_data[0]; };

protected:
  unsigned int _size = 0;
  bool _dirty = true;
  std::vector<char> _data;
};


template <class T>
class SwappableBufferObject {
public:
  typedef std::shared_ptr<T> TypePtr;
  virtual ~SwappableBufferObject() = default;

  explicit SwappableBufferObject(unsigned int count) {
    _buffers.resize(count);
  }

  void swap() {
    _currentIndex = (int) ((_currentIndex + 1) % _buffers.size());
  }
  TypePtr current() { return _buffers[_currentIndex]; }

protected:
  int _currentIndex = 0;
  std::vector<TypePtr> _buffers;

protected:
  void _createBuffers() {
    for (auto &buffer : _buffers) {
      buffer = createBuffer();
    }
  }
  virtual TypePtr createBuffer() = 0;
};

#endif //CPPWRAPPER_MEMORYBUFFER_H
