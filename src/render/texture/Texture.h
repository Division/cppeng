//
// Created by Sidorenko Nikita on 3/25/18.
//

#ifndef CPPWRAPPER_TEXTURE_H
#define CPPWRAPPER_TEXTURE_H

#include "EngineGL.h"

#include <memory>

struct TextureParams {

};

class Texture {
public:
  Texture() = default;
  ~Texture();

  explicit Texture(GLuint id): _id(id) {}
  GLuint id() const { if (!_id) { _genID(); } return _id; }
  void bind(GLenum unit) const;
  void unbind(GLenum unit) const;
  void initTexture2D(int width, int height, int channels, bool sRGB, void *data, bool mipmaps = true);
  void initTexture2D(int width, int height, GLenum format, GLenum internalFormat, GLenum type, void *data, bool mipmaps);

private:
  mutable GLuint _id = 0;

private:
  GLuint _target;
  void _genID() const;
  void _uploadData();
};

typedef std::shared_ptr<Texture> TexturePtr;

#endif //CPPWRAPPER_TEXTURE_H
