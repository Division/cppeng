//
// Created by Sidorenko Nikita on 4/20/18.
//

#include "MemoryBuffer.h"
#include "EngMath.h"
#include <stdexcept>
#include "system/Logging.h"

const int ALIGN_BYTES = 4;

void MemoryBuffer::resize(unsigned int size) {
  _size = size;
  if (size > _data.size()) {
    auto alignedSize = (unsigned int)(ceil((float)size / ALIGN_BYTES) * ALIGN_BYTES);
    _data.resize(alignedSize);
    setDirty();
  }
}

void MemoryBuffer::erase() {
  std::vector<char>().swap(_data);
  _size = 0;
}

int MemoryBuffer::writeData(void *data, unsigned int offset, unsigned int size) {
  if (offset + size > _size || size == 0) {
    throw std::invalid_argument("size bounds exceeded");
  }

  memcpy(&_data[offset], data, size);

  setDirty();

  return offset + size;
}

void MemoryBuffer::upload() {
  _dirty = false;
}

int MemoryBuffer::appendData(void *data, unsigned int size, unsigned int alignBytes) {
  auto offset = _size;
  resize(_size + size);
  writeData(data, offset, size);

  if (alignBytes > 0) {
    align(alignBytes);
  }

  return offset;
}

void MemoryBuffer::align(unsigned int bytes) {
  auto alignedSize = (unsigned int)(ceil((float)_size / bytes) * bytes);
  resize(alignedSize);
}
