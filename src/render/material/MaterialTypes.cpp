//
// Created by Sidorenko Nikita on 3/31/18.
//

#include "MaterialTypes.h"
#include "engine/EngineMain.h"
#include "render/renderer/Renderer.h"

MaterialWhite::MaterialWhite() {
  auto engine = getEngine();
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  _shader = engine->renderer()->getShaderWithCaps(caps);

  // Manually create uniforms for now
  _shader->addUniform(UniformName::ProjectionMatrix);
  _shader->addUniform(UniformName::ModelViewMatrix);
//  _shader->addUniform(UniformName::Texture0);
}
