//
// Created by Sidorenko Nikita on 4/3/18.
//

#ifndef CPPWRAPPER_UNIFORMBINDING_H
#define CPPWRAPPER_UNIFORMBINDING_H

#include "EngMath.h"
#include "render/shader/Shader.h"
#include <render/texture/Texture.h>
#include "UniformBufferStruct.h"

using namespace UBOStruct;

struct UniformBinding {
  explicit UniformBinding(UniformName name) : uniform(name) {}
  UniformName uniform;
};

struct Mat4Binding: public UniformBinding {
  explicit Mat4Binding(UniformName name) : UniformBinding(name) {}
  mat4 matrix;
};

struct Mat3Binding: public UniformBinding {
  explicit Mat3Binding(UniformName name) : UniformBinding(name) {}
  mat3 matrix;
};

struct Vec4Binding: public UniformBinding {
  explicit Vec4Binding(UniformName name) : UniformBinding(name) {}
  vec4 v;
};

struct Vec3Binding: public UniformBinding {
  explicit Vec3Binding(UniformName name) : UniformBinding(name) {}
  vec3 v;
};

struct Vec2Binding: public UniformBinding {
  explicit Vec2Binding(UniformName name) : UniformBinding(name) {}
  vec2 v;
};

struct FloatBinding: public UniformBinding {
  explicit FloatBinding(UniformName name) : UniformBinding(name) {}
  float value = 0;
};

struct TextureBinding: public UniformBinding {
  explicit TextureBinding(UniformName name) : UniformBinding(name) {}
  TexturePtr texture;
};

// Uniform Buffer Data Bindings

struct UniformBlockBinding {
  explicit UniformBlockBinding(UniformBlockName name) : block(name) {}
  UniformBlockName block;
};

struct TransformBinding: public UniformBlockBinding {
  explicit TransformBinding() : UniformBlockBinding(UniformBlockName::Transform) {}
  TransformStruct data;
  unsigned int offset;
};

#endif //CPPWRAPPER_UNIFORMBINDING_H
