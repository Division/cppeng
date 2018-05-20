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

void Texture::initTexture2D(int width, int height, int channels, void *data) {

  GLenum mode;
  switch (channels) {
    case 4:
      mode = GL_RGBA;
      break;

    case 3:
      mode = GL_RGB;
      break;

    default:
      ENGLog("Invalid channel number");
      return;
  }

  glBindTexture(GL_TEXTURE_2D, id());
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // required for proper loading
  glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  ENGLog("TEXTURE 2D LOADED, %ix%i", width, height);

//  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


  glBindTexture(GL_TEXTURE_2D, 0);
}

