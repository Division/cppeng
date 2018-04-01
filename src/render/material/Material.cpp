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

int Material::_addMat3Binding(UniformName uniform) {
  _bindings.mat3Bindings.emplace_back(Mat3Binding(uniform));
  return (int)(_bindings.mat3Bindings.size() - 1);
}

int Material::_addVec4Binding(UniformName uniform) {
  _bindings.vec4Bindings.emplace_back(Vec4Binding(uniform));
  return (int)(_bindings.vec4Bindings.size() - 1);
}

int Material::_addVec3Binding(UniformName uniform) {
  _bindings.vec3Bindings.emplace_back(Vec3Binding(uniform));
  return (int)(_bindings.vec3Bindings.size() - 1);
}

int Material::_addVec2Binding(UniformName uniform) {
  _bindings.vec2Bindings.emplace_back(Vec2Binding(uniform));
  return (int)(_bindings.vec2Bindings.size() - 1);
}

int Material::_addFloatBinding(UniformName uniform) {
  _bindings.floatBindings.emplace_back(FloatBinding(uniform));
  return (int)(_bindings.floatBindings.size() - 1);
}

int Material::_addTextureBinding(UniformName uniform) {
  _bindings.textureBindings.emplace_back(TextureBinding(uniform));
  return (int)(_bindings.textureBindings.size() - 1);
}


void Material::uploadBindings() const {
  if (!_shader) {
    return;
  }

  for (auto &binding : _bindings.mat4Bindings) {
    _shader->getUniform(binding.uniform)->setMat4(binding.matrix);
  }

  for (auto &binding : _bindings.vec4Bindings) {
    _shader->getUniform(binding.uniform)->setVec4(binding.v);
  }

  for (auto &binding : _bindings.vec3Bindings) {
    _shader->getUniform(binding.uniform)->setVec3(binding.v);
  }

}

Material::Material() {
  _modelViewBinding = _addMat4Binding(UniformName::ModelViewMatrix);
  _projectionBinding = _addMat4Binding(UniformName::ProjectionMatrix);
}
