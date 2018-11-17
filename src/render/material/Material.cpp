//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "Material.h"
#include "system/Logging.h"

void Material::setProjection(const mat4 &projection) {
  _bindings.mat4Bindings[_projectionBinding].matrix = projection;
}

void Material::setView(const mat4 &viewMatrix) {
  _bindings.mat4Bindings[_viewMatrixBinding].matrix = viewMatrix;
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

int Material::_addIntBinding(UniformName uniform) {
  _bindings.intBindings.emplace_back(IntBinding(uniform));
  return (int)(_bindings.intBindings.size() - 1);
}


int Material::_addTextureBinding(UniformName uniform) {
  _bindings.textureBindings.emplace_back(TextureBinding(uniform));

  if (UNIFORM_TEXTURE_BLOCKS.find(uniform) != UNIFORM_TEXTURE_BLOCKS.end()) {
    _bindings.textureBindings[_bindings.textureBindings.size() - 1].textureUnit = UNIFORM_TEXTURE_BLOCKS.at(uniform);
  } else {
    throw std::runtime_error("UNIFORM_TEXTURE_BLOCKS lookup failed. No texture for uniform.");
  }

  return (int)(_bindings.textureBindings.size() - 1);
}


void Material::uploadBindings() const {
  if (!_shader) {
    return;
  }

  for (auto &binding : _bindings.mat4Bindings) {
    _shader->getUniform(binding.uniform)->setMat4(binding.matrix);
  }

  for (auto &binding : _bindings.mat3Bindings) {
    _shader->getUniform(binding.uniform)->setMat3(binding.matrix);
  }

  for (auto &binding : _bindings.vec4Bindings) {
    _shader->getUniform(binding.uniform)->setVec4(binding.v);
  }

  for (auto &binding : _bindings.vec3Bindings) {
    _shader->getUniform(binding.uniform)->setVec3(binding.v);
  }

  for (auto &binding : _bindings.vec2Bindings) {
    _shader->getUniform(binding.uniform)->setVec2(binding.v);
  }

  for (auto &binding : _bindings.floatBindings) {
//    _shader->getUniform(binding.uniform)->set(binding.v);
  }

  for (auto &binding : _bindings.intBindings) {
    _shader->getUniform(binding.uniform)->setInt(binding.value);
  }

}

Material::Material() {

}

void Material::activateTextures() const {
  for (auto &binding : _bindings.textureBindings) {
    if (!binding.texture) {
      ENGLog("Error: Texture not specified for unit: %i", binding.textureUnit);
      continue;
    }

    glActiveTexture(GL_TEXTURE0 + binding.textureUnit);
    glBindTexture(GL_TEXTURE_2D, binding.texture->id());
  }
}
