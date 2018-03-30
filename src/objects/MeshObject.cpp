//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "MeshObject.h"
//#include <render/material/Material.h>

MeshObject::MeshObject() : GameObject() {
  _mesh = std::make_shared<Mesh>();
}

void MeshObject::render(IRenderer &renderer) {
//  renderer.renderMesh(*_mesh, _material, transform()->worldMatrix());
}