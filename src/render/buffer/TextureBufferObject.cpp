//
// Created by Sidorenko Nikita on 4/21/18.
//

#include "TextureBufferObject.h"
#include "system/Logging.h"
#include <memory>

// TextureBufferObject

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

// Texture2DBuffer

Texture2DBuffer::Texture2DBuffer(int targetWidth, GLenum internalFormat)
    : MemoryBuffer(), _targetWidth(targetWidth), _internalFormat(internalFormat) {
  glGenTextures(1, &_texture);

  switch (internalFormat) {
    case GL_R8:
      _format = GL_RED;
      _type = GL_UNSIGNED_BYTE;
      _pixelSizeBytes = 1;
      break;

    case GL_R16UI:
      _format = GL_RED_INTEGER;
      _type = GL_UNSIGNED_SHORT;
      _pixelSizeBytes = 2;
      break;

    case GL_R32I:
      _format = GL_RED_INTEGER;
      _type = GL_INT;
      _pixelSizeBytes = 4;
      break;

    case GL_R32UI:
      _format = GL_RED_INTEGER;
      _type = GL_UNSIGNED_INT;
      _pixelSizeBytes = 4;
      break;

    case GL_RG16UI:
      _format = GL_RG_INTEGER;
      _type = GL_UNSIGNED_SHORT;
      _pixelSizeBytes = 4;
      break;

    case GL_RG32UI:
      _format = GL_RG_INTEGER;
      _type = GL_UNSIGNED_INT;
      _pixelSizeBytes = 8;
      break;

    case GL_RGB32UI:
      _format = GL_RGB_INTEGER;
      _type = GL_UNSIGNED_INT;
      _pixelSizeBytes = 12;
      break;

    default:
      throw std::runtime_error("Unsupported internal type");
  }
}

Texture2DBuffer::~Texture2DBuffer() {
  glDeleteTextures(1, &_texture);
}

void Texture2DBuffer::bindTexture(int textureUnit) {
  if (textureUnit >= 0) {
    glActiveTexture(GL_TEXTURE0 + (unsigned int)textureUnit);
  }
  glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture2DBuffer::upload() {
  bindTexture();

  if (!_size) {
    return;
  }

  align((unsigned)_targetWidth * _pixelSizeBytes);
  auto targetHeight = (int)ceilf((float)_size / (float)_pixelSizeBytes / (float)_targetWidth); // pixel count / width = height
  if (_dimensions.x != _targetWidth || _dimensions.y < targetHeight) {
    _dimensions = ivec2(_targetWidth, targetHeight);
    ENGLog("Create texture %ix%i of size %i", _targetWidth, targetHeight, _size);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _targetWidth, targetHeight, 0, _format, _type, nullptr);
  } else {
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _targetWidth);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _targetWidth, targetHeight, _format, _type, bufferPointer());
  }

  engine::checkGLError();
}

// SwappableTextureBufferObject

std::shared_ptr<Texture2DBuffer> SwappableTexture2DBuffer::createBuffer() {
  return std::make_shared<Texture2DBuffer>(_targetWidth, _internalFormat);
}

std::shared_ptr<TextureBufferObject> SwappableTextureBufferObject::createBuffer() {
  return std::make_shared<TextureBufferObject>(_format, _usage);
}

