//
// Created by Sidorenko Nikita on 11/26/18.
//

#include "PostEffect.h"
#include "EngineMain.h"
#include "render/shader/ShaderGenerator.h"
#include "render/renderer/Renderer.h"
#include "EngineGL.h"
#include "render/material/MaterialTypes.h"
#include "render/shader/Uniform.h"
#include "utils/MeshGeneration.h"

PostEffect::PostEffect(std::shared_ptr<Renderer> renderer) {
  _renderer = renderer;

  auto generator = getEngine()->shaderGenerator();
  generator->addTemplate("posteffect/root.tpl");
  ShaderCapsSetPtr caps = std::make_shared<ShaderCapsSet>();
  _shader = generator->getShaderWithCaps(caps, "posteffect/root.tpl");
  _shader->addUniform(UniformName::Texture0);

  _fullScreenQuad = std::make_shared<Mesh>();
  MeshGeneration::generateFullScreenQuad(_fullScreenQuad);
  _fullScreenQuad->createBuffer();
}

void PostEffect::render(TexturePtr source, std::shared_ptr<ICameraParamsProvider> camera) {
  source->bind(0);
  _shader->bind();

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  _renderer->renderMesh(_fullScreenQuad, GL_TRIANGLES);

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  source->unbind(0);
  _shader->unbind();
}
