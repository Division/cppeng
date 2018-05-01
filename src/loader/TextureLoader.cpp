//
// Created by Sidorenko Nikita on 3/25/18.
//

#include "TextureLoader.h"

#ifndef __EMSCRIPTEN__
#include "SDL.h"
#include "SDL_image.h"
#endif

#include "system/Logging.h"

TexturePtr loader::loadTexture(const std::string &name) {
  TexturePtr tex(new Texture());
#ifndef __EMSCRIPTEN__
  SDL_Surface *surface;
  SDL_RWops *rwop;
  rwop = SDL_RWFromFile(name.c_str(), "rb");

  surface = IMG_LoadPNG_RW(rwop);
  if (surface) {
    tex->initWithSDLSurface(surface);
  } else {
    ENGLog("Error loading texture");
  }
#endif
  return tex;
};