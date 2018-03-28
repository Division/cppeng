//
// Created by Sidorenko Nikita on 3/25/18.
//

#include "TextureLoader.h"
#include "SDL.h"
#include "SDL_image.h"
#include "system/Logging.h"

TexturePtr loader::loadTexture(const std::string &name) {
  SDL_Surface *surface;
  SDL_RWops *rwop;
  rwop = SDL_RWFromFile(name.c_str(), "rb");

  TexturePtr tex(new Texture());
  surface = IMG_LoadPNG_RW(rwop);
  if (surface) {
    tex->initWithSDLSurface(surface);
  } else {
    ENGLog("Error loading texture");
  }

  return tex;
};