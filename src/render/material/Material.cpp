//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "Material.h"
#include "system/Logging.h"
#include <vector>
#include "EngineMain.h"
#include <unordered_set>

// Data structures needed to automatically setup shader uniforms and uniform blocks by the ShaderCapsSet

struct ShaderConfig {
  std::vector<UniformName > uniforms;
  std::vector<UniformBlockName> uniformBlocks;
};

const std::vector<UniformBlockName> DEFAULT_UBO = { UniformBlockName::Transform, UniformBlockName::Camera };

const std::map<ShaderCaps, ShaderConfig> UNIFORMS_PER_CAP = {
    { ShaderCaps::Color, { { UniformName::Color }, DEFAULT_UBO } },
    { ShaderCaps::Lighting, {
      { UniformName::ShadowMap, UniformName::LightGrid, UniformName::LightIndices, UniformName::ProjectorTexture},
      { UniformBlockName::Transform, UniformBlockName::Camera, UniformBlockName::Light, UniformBlockName::Projector }
    } },
    { ShaderCaps::NormalMap, { { UniformName::NormalMap }, DEFAULT_UBO } },
    { ShaderCaps::Texture0, { { UniformName::Texture0 }, DEFAULT_UBO } },
    { ShaderCaps::VertexColor, { {}, DEFAULT_UBO } },
    { ShaderCaps::SpecularMap, { { UniformName::SpecularMap }, {} } },
    { ShaderCaps::TerrainLayer0, { { UniformName::TerrainDiffuse0, UniformName::TerrainNormal0 }, { DEFAULT_UBO } } },
    { ShaderCaps::TerrainLayer1, { { UniformName::TerrainDiffuse1, UniformName::TerrainNormal1, UniformName::TerrainSplatmap }, {} } },
    { ShaderCaps::TerrainLayer2, { { UniformName::TerrainDiffuse2, UniformName::TerrainNormal2, UniformName::TerrainSplatmap }, {} } }
};

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

  std::unordered_set<UniformName> uniforms;
  std::unordered_set<UniformBlockName> uniformBlocks;

  auto appendUniforms = [&](ShaderCaps cap) {
    auto &config = UNIFORMS_PER_CAP.at(cap);
    for (auto &name : config.uniforms) {
      uniforms.insert(name);
    }
    for (auto &name : config.uniformBlocks) {
      uniformBlocks.insert(name);
    }
  };

  // Auto adding uniforms and blocks
  for (auto iterator : UNIFORMS_PER_CAP) {
    if (caps->hasCap(iterator.first)) { appendUniforms(iterator.first); }
  }

  auto engine = getEngine();

  _shader = engine->getShaderWithCaps(caps);
//  caps->addCap(ShaderCaps::Skinning);
//  _shaderSkinning = engine->getShaderWithCaps(caps);
//  _shaderSkinning->addUniformBlock(UniformBlockName::SkinningMatrices);

  for (auto name : uniforms) {
    _shader->addUniform(name);
//    _shaderSkinning->addUniform(name);
  }

  for (auto name : uniformBlocks) {
    _shader->addUniformBlock(name);
//    _shaderSkinning->addUniformBlock(name);
  }

  ShaderCapsSetPtr depthOnlyCaps = std::make_shared<ShaderCapsSet>(); // empty caps
  _shaderDepthOnly = engine->getShaderWithCaps(depthOnlyCaps);
  _shaderDepthOnly->addUniformBlock(UniformBlockName::Transform);
  _shaderDepthOnly->addUniformBlock(UniformBlockName::Camera);
//
//  depthOnlyCaps->addCap(ShaderCaps::Skinning);
//  _shaderDepthOnlySkinning = engine->getShaderWithCaps(depthOnlyCaps);
//  _shaderDepthOnlySkinning->addUniformBlock(UniformBlockName::Transform);
//  _shaderDepthOnlySkinning->addUniformBlock(UniformBlockName::Camera);
//  _shaderDepthOnlySkinning->addUniformBlock(UniformBlockName::SkinningMatrices);
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
