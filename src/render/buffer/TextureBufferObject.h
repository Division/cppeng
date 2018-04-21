//
// Created by Sidorenko Nikita on 4/21/18.
//

#ifndef CPPWRAPPER_TEXTUREBUFFEROBJECT_H
#define CPPWRAPPER_TEXTUREBUFFEROBJECT_H

#include "EngineGL.h"
#include "VertexBufferObject.h"
#include "MemoryBuffer.h"
#include "render/texture/TextureSlot.h"

class TextureBufferObject : public VertexBufferObject {
public:
  TextureBufferObject(GLenum format, GLenum usage);
  ~TextureBufferObject() override;
  void bindTexture(TextureSlot slot = TextureSlot::None);
  GLenum textureID() { return _texture; }

private:
  GLenum _texture;
  GLenum _format;
};

class SwappableTextureBufferObject : public SwappableBufferObject<TextureBufferObject> {
public:
  SwappableTextureBufferObject(GLenum format, GLenum usage)
      : _format(format), _usage(usage), SwappableBufferObject<TextureBufferObject>(2) {
    _createBuffers();
  }

protected:
  GLenum _format;
  GLenum _usage;
  std::shared_ptr<TextureBufferObject> createBuffer() override;
};

#endif //CPPWRAPPER_TEXTUREBUFFEROBJECT_H
