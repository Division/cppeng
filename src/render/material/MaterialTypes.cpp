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
  _shader->addUniform(UniformName::ModelViewMatrix);
  _shader->addUniform(UniformName::Color);

  color(vec4(1, 1, 1, 1));
}
