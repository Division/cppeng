//
// Created by Sidorenko Nikita on 3/31/18.
//

#include "MaterialTypes.h"
#include "EngineMain.h"
#include "render/renderer/Renderer.h"


MaterialSingleColor::MaterialSingleColor() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Color);
  _setup(caps);

  _colorBinging = _addVec4Binding(UniformName::Color);
  color(vec4(1, 1, 1, 1));
}

MaterialDebug::MaterialDebug() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::PointSize);
  caps->addCap(ShaderCaps::VertexColor);
  _setup(caps);
}

MaterialBillboard::MaterialBillboard() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  caps->addCap(ShaderCaps::Color);
  caps->addCap(ShaderCaps::Billboard);

//  _supportsSkinning = false;
  _setup(caps);

//  auto generator = getEngine()->shaderGenerator();
//  generator->addTemplate("effects/billboard.glsl");
//  _shader = generator->getShaderWithCaps(caps, "effects/billboard.glsl");

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
  _colorBinging = _addVec4Binding(UniformName::Color);
  color(vec4(1, 1, 1, 1));
}

MaterialTexture::MaterialTexture() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  _setup(caps);

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
}

MaterialTextureLighting::MaterialTextureLighting() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  caps->addCap(ShaderCaps::Lighting);
  _setup(caps);

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
}

MaterialTextureSpecularMap::MaterialTextureSpecularMap() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::SpecularMap);
  _setup(caps);

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
  _specularmapBinding = _addTextureBinding(UniformName::SpecularMap);
}

MaterialTextureBump::MaterialTextureBump() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::NormalMap);
  _setup(caps);

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
  _normalmapBinding = _addTextureBinding(UniformName::NormalMap);
}

MaterialTextureBumpSpecular::MaterialTextureBumpSpecular() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Texture0);
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::NormalMap);
  caps->addCap(ShaderCaps::SpecularMap);
  _setup(caps);

  _texture0Binding = _addTextureBinding(UniformName::Texture0);
  _normalmapBinding = _addTextureBinding(UniformName::NormalMap);
  _specularmapBinding = _addTextureBinding(UniformName::SpecularMap);
}

MaterialLighting::MaterialLighting() {
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Lighting);
  _setup(caps);
}

MaterialTerrain::MaterialTerrain(int layerCount, bool specularmap) {
  auto engine = getEngine();

  _supportsSkinning = false; // no skinning for terrain

  _diffuseBindings.resize(layerCount);
  _normalMapBindings.resize(layerCount);

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::NormalMap);

  if (specularmap) {
    caps->addCap(ShaderCaps::SpecularMap);
    _specularmapBinging = _addTextureBinding(UniformName::SpecularMap);
  }

  if (layerCount > 1) {
    _splatmapBinging = _addTextureBinding(UniformName::TerrainSplatmap);
  }

  for (int i = 0; i < layerCount; i++) {
    auto capsOffset = (int)ShaderCaps::TerrainLayer0 + i;
    caps->addCap((ShaderCaps)capsOffset);

    auto diffuseOffset = (int)UniformName::TerrainDiffuse0 + i;
    _diffuseBindings[i] = _addTextureBinding((UniformName)diffuseOffset);

    auto normalMapOffset = (int)UniformName::TerrainNormal0 + i;
    _normalMapBindings[i] = _addTextureBinding((UniformName)normalMapOffset);
  }

  _setup(caps);
}

// NOTE: material for testing
// Actual projection rendering is built into the render pipeline
MaterialTextureProjection::MaterialTextureProjection() {
  auto engine = getEngine();

  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  caps->addCap(ShaderCaps::Lighting);
  caps->addCap(ShaderCaps::ProjectedTexture);
  _shader = engine->getShaderWithCaps(caps);

  _projectedTextureBinding = _addTextureBinding(UniformName::ProjectedTexture);
  _projectedTextureMatrixBinding = _addMat4Binding(UniformName::ProjectedTextureMatrix);

  // Manually create uniforms
  _shader->addUniform(UniformName::LightGrid);
  _shader->addUniform(UniformName::LightIndices);
  _shader->addUniform(UniformName::ProjectedTexture);
  _shader->addUniform(UniformName::ProjectedTextureMatrix);

  _shader->addUniformBlock(UniformBlockName::ObjectParams);
  _shader->addUniformBlock(UniformBlockName::Light);
  _shader->addUniformBlock(UniformBlockName::Camera);
}


