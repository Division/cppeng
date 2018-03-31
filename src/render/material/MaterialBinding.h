//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_MATERIALBINDING_H
#define CPPWRAPPER_MATERIALBINDING_H

#include "EngMath.h"
#include <vector>
#include "render/shader/Shader.h"

struct UniformBinding {
  explicit UniformBinding(UniformName name) : uniform(name) {}
  UniformName uniform;
};

struct Mat4Binding: public UniformBinding {
  explicit Mat4Binding(UniformName name) : UniformBinding(name) {}
  mat4 matrix;
};

class MaterialBingings {
public:
  std::vector<Mat4Binding> mat4Bindings;
};


#endif //CPPWRAPPER_MATERIALBINDING_H
