//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_MATERIALBINDING_H
#define CPPWRAPPER_MATERIALBINDING_H

#include "EngMath.h"
#include <vector>
#include "render/shader/Shader.h"
#include <render/texture/Texture.h>

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

class MaterialBingings {
public:
  std::vector<Mat4Binding> mat4Bindings;
  std::vector<Mat3Binding> mat3Bindings;
  std::vector<Vec4Binding> vec4Bindings;
  std::vector<Vec3Binding> vec3Bindings;
  std::vector<Vec2Binding> vec2Bindings;
  std::vector<FloatBinding> floatBindings;
  std::vector<TextureBinding> textureBindings;
};


#endif //CPPWRAPPER_MATERIALBINDING_H
