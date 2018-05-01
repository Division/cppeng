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
  _shader->addUniform(UniformName::ProjectionMatrix);
  _shader->addUniform(UniformName::ViewMatrix);
  _shader->addUniform(UniformName::Color);
  _shader->addUniformBlock(UniformBlockName::Transform);

  color(vec4(1, 1, 1, 1));
}

MaterialLighting::MaterialLighting() {
  auto engine = getEngine();

//  auto colorBinding = _addVec4Binding(UniformName::Color);
//  _bindings.vec4Bindings[colorBinding].v = vec4(1,1, 0,1);


  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
//  caps->addCap(ShaderCaps::Color);
  caps->addCap(ShaderCaps::Lighting);
  _shader = engine->renderer()->getShaderWithCaps(caps);

  // Manually create uniforms for now
  _shader->addUniform(UniformName::ProjectionMatrix);
  _shader->addUniform(UniformName::ViewMatrix);
  _shader->addUniform(UniformName::LightGrid);
  _shader->addUniform(UniformName::LightIndices);

  _shader->addUniformBlock(UniformBlockName::Transform);
  _shader->addUniformBlock(UniformBlockName::Light);
  _shader->addUniformBlock(UniformBlockName::Camera);
}
