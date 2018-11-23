//
// Created by Sidorenko Nikita on 11/22/18.
//

#ifndef CPPWRAPPER_MULTIVERTEXBUFFEROBJECT_H
#define CPPWRAPPER_MULTIVERTEXBUFFEROBJECT_H

#include <vector>
#include <memory>

#include "VertexBufferObject.h"

struct MultiVBOAddress {
  unsigned int index = 0; // index of the VBO
  unsigned int offset = 0; // offset within VBO
};

// Some buffers (e.g. UBO) have limited maximum capacity
// To handle all required data, multiple buffers should be allocated
// This class uses glMapBuffer / glUnmapBuffer for writing data
// Each buffer size is fixed (for UBO should be equal to max UBO size)
//
// IMPORTANT: this class assumes that no buffer binding to _target occurs between map and unmap calls.
class MultiVertexBufferObject {
public:
  MultiVertexBufferObject(GLenum target, GLenum usage, unsigned int bufferSize, unsigned int alignment, GLenum access = GL_WRITE_ONLY) :
      _target(target), _usage(usage), _bufferSize(bufferSize), _alignment(alignment), _access(access) {}

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
  int _currentIndex = 0;
  bool _isMapped = false;
  char *_mappedPointer;

  std::vector<SwappableVertexBufferObjectPtr> _buffers;

  void _setCurrentBufferMapping(bool mapped);
  void _nextVBO();
};


#endif //CPPWRAPPER_MULTIVERTEXBUFFEROBJECT_H
