//
// Created by Sidorenko Nikita on 3/25/18.
//

#include "Texture.h"
#include "system/Logging.h"

void Texture::_uploadData() {

}

void Texture::_genID() const {
  if (_id) { return; };
  glGenTextures(1, &_id);
}

void Texture::initTexture2D(int width, int height, int channels, bool sRGB, void *data, bool mipmaps) {
  GLenum internalFormat;
  GLenum format;
  switch (channels) {
    case 4:
      format = GL_RGBA;
      internalFormat = sRGB ? GL_SRGB8_ALPHA8: GL_RGBA;
      break;

    case 3:
      format = GL_RGB;
      internalFormat = sRGB ? GL_SRGB8 : GL_RGB;
      break;

    default:
      throw std::runtime_error("Invalid texture channel number");
  }

  this->initTexture2D(width, height, format, internalFormat, GL_UNSIGNED_BYTE, data, mipmaps);
}

void Texture::initTexture2D(int width, int height, GLenum format, GLenum internalFormat, GLenum type, void *data, bool mipmaps) {
  _target = GL_TEXTURE_2D;

  glBindTexture(GL_TEXTURE_2D, id());
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // required for proper loading
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

  bool cpuMipMap = false;
  if (mipmaps) {
#ifdef __EMSCRIPTEN__
    if (sRGB) { // sRGB mips can only be generated on CPU in webgl build
      ENGLog("Skipped mipmap because webgl doesn't support sRGB mip generation");
      cpuMipMap = true;
    }
#endif

    if (!cpuMipMap) {
      ENGLog("Generating mipmap... %ix%i", width, height);
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      engine::checkGLError();
    }
  }

  if (!mipmaps || cpuMipMap) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  engine::checkGLError();

  ENGLog("TEXTURE 2D LOADED, %ix%i", width, height);


  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
  glBindTexture(GL_TEXTURE_2D, id());
  glDeleteTextures(1, &_id);
}

void Texture::bind(GLenum unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(_target, _id);
}

void Texture::unbind(GLenum unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(_target, 0);
}

