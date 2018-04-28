//
// Created by Sidorenko Nikita on 4/21/18.
//

#include "TextureBufferObject.h"
#include "system/Logging.h"
#include <memory>

TextureBufferObject::TextureBufferObject(GLenum format, GLenum usage)
    : VertexBufferObject(GL_TEXTURE_BUFFER, usage), _format(format) {
  glGenTextures(1, &_texture);
  bindTexture();
  bind();
  glTexBuffer(GL_TEXTURE_BUFFER, _format, _vbo);
}

TextureBufferObject::~TextureBufferObject() {
  glDeleteTextures(1, &_texture);
}

void TextureBufferObject::bindTexture(int textureUnit) {
  if (textureUnit >= 0) {
    glActiveTexture(GL_TEXTURE0 + (unsigned int)textureUnit);
  }
  glBindTexture(GL_TEXTURE_BUFFER, _texture);
}

std::shared_ptr<TextureBufferObject> SwappableTextureBufferObject::createBuffer() {
  return std::make_shared<TextureBufferObject>(_format, _usage);
}

