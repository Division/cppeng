//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "MeshObject.h"
//#include <render/material/Material.h>

MeshObject::MeshObject() : GameObject() {

}

void MeshObject::render(IRenderer &renderer) {
  if (!_mesh || !_material) {
    return;
  }

  renderer.renderMesh(*_mesh, *_material, transform()->worldMatrix());
}