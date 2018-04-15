//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_MATERIALBINDING_H
#define CPPWRAPPER_MATERIALBINDING_H

#include <vector>
#include "render/shader/UniformBinding.h"

class MaterialBingings {
public:
  std::vector<Mat4Binding> mat4Bindings;
  std::vector<Mat3Binding> mat3Bindings;
  std::vector<Vec4Binding> vec4Bindings;
  std::vector<Vec3Binding> vec3Bindings;
  std::vector<Vec2Binding> vec2Bindings;
  std::vector<FloatBinding> floatBindings;
  std::vector<TextureBinding> textureBindings;

  // Uniform blocks

  bool hasTransform = true;
  TransformBinding transform;
};


#endif //CPPWRAPPER_MATERIALBINDING_H
