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

#ifndef __EMSCRIPTEN__
void Texture::initWithSDLSurface(SDL_Surface *surface) {
  glBindTexture(GL_TEXTURE_2D, id());

  GLenum mode = GL_RGB;

  if(surface->format->BytesPerPixel == 4) {
    mode = GL_RGBA;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

  ENGLog("TEX LOADED, %i/%i", surface->w, surface->h);

//  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);
}
#endif
