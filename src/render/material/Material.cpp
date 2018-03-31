//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "Material.h"

void Material::setModelView(const mat4 &modelView) {
  _bindings.mat4Bindings[_modelViewBinding].matrix = modelView;
}

void Material::setProjection(const mat4 &projection) {
  _bindings.mat4Bindings[_projectionBinding].matrix = projection;
}

int Material::_addMat4Binding(UniformName uniform) {
  _bindings.mat4Bindings.emplace_back(Mat4Binding(uniform));
  return (int)(_bindings.mat4Bindings.size() - 1);
}

void Material::uploadBindings() const {
  if (!_shader) {
    return;
  }

  for (auto &binding : _bindings.mat4Bindings) {
    _shader->getUniform(binding.uniform)->setMatrix(binding.matrix);
  }
}

Material::Material() {
  _modelViewBinding = _addMat4Binding(UniformName::ModelViewMatrix);
  _projectionBinding = _addMat4Binding(UniformName::ProjectionMatrix);
}
