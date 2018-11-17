//
// Created by Sidorenko Nikita on 3/31/18.
//

#include "MaterialTypes.h"
#include "engine/EngineMain.h"
#include "render/renderer/Renderer.h"

MaterialSingleColor::MaterialSingleColor() {
  auto engine = getEngine();

  _colorBinging = _addVec4Binding(UniformName::Color);

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Color);
  _shader = engine->renderer()->getShaderWithCaps(caps);

  // Manually create uniforms for now
  _shader->addUniform(UniformName::Color);
  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Camera);

  color(vec4(1, 1, 1, 1));
}

MaterialDebug::MaterialDebug() {
  auto engine = getEngine();

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::PointSize);
  caps->addCap(ShaderCaps::VertexColor);
  _shader = engine->renderer()->getShaderWithCaps(caps);

  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Camera);
}

MaterialTexture::MaterialTexture() {
  auto engine = getEngine();

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
//  _texture1Binding = _addTextureBinding(UniformName::Texture1);

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  _shader = engine->renderer()->getShaderWithCaps(caps);

  // Manually create uniforms for now
  _shader->addUniform(UniformName::Texture0);
//  _shader->addUniform(UniformName::Texture1);
//  _shader->addUniform(UniformName::ProjectorTexture);
  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Camera);
}

MaterialLighting::MaterialLighting() {
  auto engine = getEngine();

//  auto projectorTextureBinding = _addIntBinding(UniformName::ProjectorTexture);
//  _bindings.intBindings[projectorTextureBinding].value = UNIFORM_TEXTURE_BLOCKS.at(UniformName::ProjectorTexture);

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Lighting);
  _shader = engine->renderer()->getShaderWithCaps(caps);

  // Manually create uniforms for now
  _shader->addUniform(UniformName::LightGrid);
  _shader->addUniform(UniformName::LightIndices);
  _shader->addUniform(UniformName::ProjectorTexture);

  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Light);
  _shader->addUniformBlock(UniformBlockName::Projector);
  _shader->addUniformBlock(UniformBlockName::Camera);
}

MaterialTerrain::MaterialTerrain(int layerCount, bool specularmap) {
  auto engine = getEngine();

  _diffuseBindings.resize(layerCount);
  _normalMapBindings.resize(layerCount);

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::NormalMap);

  if (specularmap) {
    caps->addCap(ShaderCaps::SpecularMap);
    _specularmapBinging = _addTextureBinding(UniformName::SpecularMap);
  }

  for (int i = 0; i < layerCount; i++) {
    auto capsOffset = (int)ShaderCaps::TerrainLayer0 + i;
    caps->addCap((ShaderCaps)capsOffset);

    auto diffuseOffset = (int)UniformName::TerrainDiffuse0 + i;
    _diffuseBindings[i] = _addTextureBinding((UniformName)diffuseOffset);

    auto normalMapOffset = (int)UniformName::TerrainNormal0 + i;
    _normalMapBindings[i] = _addTextureBinding((UniformName)normalMapOffset);
  }


  _shader = engine->renderer()->getShaderWithCaps(caps);

  if (specularmap) {
    _shader->addUniform(UniformName::SpecularMap);
  }

  for (int i = 0; i < layerCount; i++) {
    auto diffuseOffset = (int)UniformName::TerrainDiffuse0 + i;
    _shader->addUniform((UniformName)diffuseOffset);

    auto normalMapOffset = (int)UniformName::TerrainNormal0 + i;
    _shader->addUniform((UniformName)normalMapOffset);
  }

  if (layerCount > 1) {
    _splatmapBinging = _addTextureBinding(UniformName::TerrainSplatmap);
    _shader->addUniform(UniformName::TerrainSplatmap);
  }

  _shader->addUniform(UniformName::LightGrid);
  _shader->addUniform(UniformName::LightIndices);
  _shader->addUniform(UniformName::ProjectorTexture);

  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Light);
  _shader->addUniformBlock(UniformBlockName::Projector);
  _shader->addUniformBlock(UniformBlockName::Camera);
}

MaterialTextureProjection::MaterialTextureProjection() {
  auto engine = getEngine();

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::ProjectedTexture);
  _shader = engine->renderer()->getShaderWithCaps(caps);

  _projectedTextureBinding = _addTextureBinding(UniformName::ProjectedTexture);
  _projectedTextureMatrixBinding= _addMat4Binding(UniformName::ProjectedTextureMatrix);

  // Manually create uniforms for now
  _shader->addUniform(UniformName::LightGrid);
  _shader->addUniform(UniformName::LightIndices);
  _shader->addUniform(UniformName::ProjectedTexture);
  _shader->addUniform(UniformName::ProjectedTextureMatrix);

  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Light);
  _shader->addUniformBlock(UniformBlockName::Camera);
}