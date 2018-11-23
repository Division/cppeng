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

void Texture::initTexture2D(int width, int height, int channels, bool sRGB, void *data) {

  GLenum internalFormat;
  GLenum format;
  switch (channels) {
    case 4:
      format = GL_RGBA;
      internalFormat = sRGB ? GL_SRGB8_ALPHA8: GL_RGBA;
//      internalFormat = GL_RGBA;
      break;

    case 3:
      format = GL_RGB;
      internalFormat = sRGB ? GL_SRGB8 : GL_RGB;
//      internalFormat = GL_RGB;
      break;

    default:
      throw std::runtime_error("Invalid texture channel number");
  }

  bool cpuMipMap = false;
#ifdef __EMSCRIPTEN__
  if (sRGB) { // sRGB mips can only be generated on CPU in webgl build
    ENGLog("Skipped mipmap because webgl doesn't support SRGB");
    cpuMipMap = true;
  }
#endif

  glBindTexture(GL_TEXTURE_2D, id());
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // required for proper loading
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  if (!cpuMipMap) {
    ENGLog("Generating mipmap... %ix%i", width, height);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    engine::checkGLError();
  } else {
    // TODO: generate CPU mipmaps
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

