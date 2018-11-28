//
// Created by Sidorenko Nikita on 11/25/18.
//

#ifndef CPPWRAPPER_FRAMEBUFFEROBJECT_H
#define CPPWRAPPER_FRAMEBUFFEROBJECT_H

#include "EngineGL.h"
#include "EngTypes.h"
#include <memory>
#include <string>
#include "MemoryBuffer.h"

class FrameBufferObject {
public:
  FrameBufferObject(unsigned int width, unsigned int height, bool color, bool depth);
  ~FrameBufferObject();

  TexturePtr colorBuffer() { return _colorBuffer; };
  TexturePtr depthBuffer() { return _depthBuffer; };

  GLuint id() { return _fboID; }
  void bind();
  void unbind();
private:
  unsigned int width;
  unsigned int height;
  GLuint _fboID;
  TexturePtr _colorBuffer;
  TexturePtr _depthBuffer;

private:
  std::string _getStatusString(GLenum status);
};

typedef std::shared_ptr<FrameBufferObject> FrameBufferObjectPtr;

class SwappableFrameBufferObject : public SwappableBufferObject<FrameBufferObject> {
public:
  SwappableFrameBufferObject(unsigned int width, unsigned int height, bool color, bool depth, unsigned int count = 2)
      : SwappableBufferObject<FrameBufferObject>(count), _width(width), _height(height), _color(color), _depth(depth) {
    _createBuffers();
  }

protected:
  std::shared_ptr<FrameBufferObject> createBuffer() override;

private:
  unsigned int _width;
  unsigned int _height;
  bool _color;
  bool _depth;
};


#endif //CPPWRAPPER_FRAMEBUFFEROBJECT_H
