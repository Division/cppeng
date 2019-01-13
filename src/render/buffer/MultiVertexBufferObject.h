//
// Created by Sidorenko Nikita on 11/22/18.
//

#ifndef CPPWRAPPER_MULTIVERTEXBUFFEROBJECT_H
#define CPPWRAPPER_MULTIVERTEXBUFFEROBJECT_H

#include <vector>
#include <memory>
#include "MultiVBOAddress.h"
#include "VertexBufferObject.h"

#define USE_MEMORY_BUFFER_MAP true

// Some buffers (e.g. UBO) have limited maximum capacity
// To handle all required data, multiple buffers should be allocated
// This class does it by creating buffers when required and mapping data
// by calling glMapBuffer / glUnmapBuffer
// Each buffer size is fixed (for UBO should be equal to max UBO size)
//
// IMPORTANT: this class assumes that no buffer binding to _target occurs between map() and unmap() calls.
// Also, next vbo is picked after each map() call
class MultiVertexBufferObject {
public:
  MultiVertexBufferObject(GLenum target, GLenum usage, unsigned int bufferSize, unsigned int alignment, GLenum access = GL_MAP_WRITE_BIT) :
      _target(target), _usage(usage), _access(access), _bufferSize(bufferSize), _alignment(alignment) {}

  unsigned int getVBO(unsigned int index);
  void map();
  void unmap();
  void swapBuffers();
  MultiVBOAddress appendData(void *data, unsigned int size);
private:
  GLenum _target;
  GLenum _usage;
  GLenum _access;
  unsigned int _bufferSize;
  unsigned int _alignment;
  unsigned int _currentOffset = 0;
  int _currentIndex = -1;
  bool _isMapped = false;
  char *_mappedPointer = nullptr;

  std::vector<SwappableVertexBufferObjectPtr> _buffers;

  void _setCurrentBufferMapping(bool mapped);
  void _nextVBO();
};


#endif //CPPWRAPPER_MULTIVERTEXBUFFEROBJECT_H
