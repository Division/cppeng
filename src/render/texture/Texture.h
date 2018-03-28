//
// Created by Sidorenko Nikita on 3/25/18.
//

#ifndef CPPWRAPPER_TEXTURE_H
#define CPPWRAPPER_TEXTURE_H

#include "EngineGL.h"
#include "SDL.h"
#include <memory>

struct TextureParams {

};

class Texture {
public:
  GLuint id() const { if (!_id) { _genID(); } return _id; }
  void initWithSDLSurface(SDL_Surface *surface);

private:
  mutable GLuint _id = 0;

private:
  void _genID() const;
  void _uploadData();
};

typedef std::shared_ptr<Texture> TexturePtr;

#endif //CPPWRAPPER_TEXTURE_H
