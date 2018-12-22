//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "Material.h"
#include "system/Logging.h"
#include <vector>
#include "EngineMain.h"
#include <unordered_set>

// Data structures needed to automatically setup shader uniforms and uniform blocks by the ShaderCapsSet

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


void Material::uploadBindings(ShaderPtr shader) const {
  if (!shader) {
    return;
  }

  for (auto &binding : _bindings.mat4Bindings) {
    shader->getUniform(binding.uniform)->setMat4(binding.matrix);
  }

  for (auto &binding : _bindings.mat3Bindings) {
    shader->getUniform(binding.uniform)->setMat3(binding.matrix);
  }

  for (auto &binding : _bindings.vec4Bindings) {
    shader->getUniform(binding.uniform)->setVec4(binding.v);
  }

  for (auto &binding : _bindings.vec3Bindings) {
    shader->getUniform(binding.uniform)->setVec3(binding.v);
  }

  for (auto &binding : _bindings.vec2Bindings) {
    shader->getUniform(binding.uniform)->setVec2(binding.v);
  }

  for (auto &binding : _bindings.floatBindings) {
//    shader->getUniform(binding.uniform)->set(binding.v);
  }

  for (auto &binding : _bindings.intBindings) {
    shader->getUniform(binding.uniform)->setInt(binding.value);
  }

}

void Material::_setup(ShaderCapsSetPtr caps) {
  if (caps->hasCap(ShaderCaps::Skinning)) {
    throw std::runtime_error("Materials must not specify ShaderCaps::Skinning");
  }

  auto engine = getEngine();

  _shader = engine->getShaderWithCaps(caps);
  caps->addCap(ShaderCaps::Skinning);
  _shaderSkinning = engine->getShaderWithCaps(caps);

  ShaderCapsSetPtr depthOnlyCaps = std::make_shared<ShaderCapsSet>(); // empty caps
  depthOnlyCaps->addCap(ShaderCaps::ObjectData);
  _shaderDepthOnly = engine->getShaderWithCaps(depthOnlyCaps);

  depthOnlyCaps->addCap(ShaderCaps::Skinning);
  _shaderDepthOnlySkinning = engine->getShaderWithCaps(depthOnlyCaps);
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
