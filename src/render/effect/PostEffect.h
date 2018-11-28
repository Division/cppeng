//
// Created by Sidorenko Nikita on 11/26/18.
//

#ifndef CPPWRAPPER_POSTEFFECT_H
#define CPPWRAPPER_POSTEFFECT_H

#include "EngTypes.h"
#include <memory>
#include <render/mesh/Mesh.h>
#include "render/renderer/ICameraParamsProvider.h"
#include "EngTypes.h"

class Renderer;

class PostEffect {
public:
  PostEffect(std::shared_ptr<Renderer> renderer);

  void render(TexturePtr source, std::shared_ptr<ICameraParamsProvider> camera);

private:
  std::shared_ptr<Renderer> _renderer;
  ShaderPtr _shader;
  MeshPtr _fullScreenQuad;
};


#endif //CPPWRAPPER_POSTEFFECT_H
