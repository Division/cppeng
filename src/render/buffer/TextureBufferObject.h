//
// Created by Sidorenko Nikita on 4/21/18.
//

#ifndef CPPWRAPPER_TEXTUREBUFFEROBJECT_H
#define CPPWRAPPER_TEXTUREBUFFEROBJECT_H

#include "EngineGL.h"
#include "VertexBufferObject.h"
#include "MemoryBuffer.h"
#include "render/texture/TextureSlot.h"
#include "EngMath.h"

// Texture which data is supplied by VBO
class TextureBufferObject : public VertexBufferObject {
public:
  TextureBufferObject(GLenum format, GLenum usage);
  ~TextureBufferObject() override;
  void bindTexture(int textureUnit = -1);
  GLenum textureID() { return _texture; }

private:
  GLenum _texture;
  GLenum _format;
};

// On WebGL texture buffers are not supported.
// This class provides interface similar to TextureBufferObject
// The difference is that data is stored in a regular texture instead of VBO
// Updates are done with glTexSubImage2D
class Texture2DBuffer: public MemoryBuffer {
public:
  Texture2DBuffer(int targetWidth, GLenum internalFormat);
  ~Texture2DBuffer() override;
  void bindTexture(int textureUnit = -1);
  void upload() override;
  void targetWidth(int width) { _targetWidth = width; }
  int targetWidth() { return _targetWidth; }
  GLenum textureID() { return _texture; }
private:
  int _targetWidth;
  int _pixelSizeBytes;
  ivec2 _dimensions = ivec2(0,0);
  GLenum _texture;
  GLenum _internalFormat;
  GLenum _format;
  GLenum _type;
};

// Swappable buffer contains several buffer instances and allow switching between them

class SwappableTexture2DBuffer: public SwappableBufferObject<Texture2DBuffer> {
public:
  SwappableTexture2DBuffer(int targetWidth, GLenum internalFormat)
      : SwappableBufferObject<Texture2DBuffer>(2), _targetWidth(targetWidth), _internalFormat(internalFormat)  {
    _createBuffers();
  }

protected:
  int _targetWidth;
  GLenum _internalFormat;
  std::shared_ptr<Texture2DBuffer> createBuffer() override;
};

class SwappableTextureBufferObject : public SwappableBufferObject<TextureBufferObject> {
public:
  SwappableTextureBufferObject(GLenum format, GLenum usage)
      :SwappableBufferObject<TextureBufferObject>(2), _format(format), _usage(usage) {
    _createBuffers();
  }

protected:
  GLenum _format;
  GLenum _usage;
  std::shared_ptr<TextureBufferObject> createBuffer() override;
};

#endif //CPPWRAPPER_TEXTUREBUFFEROBJECT_H
