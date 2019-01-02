//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "MeshObject.h"
//#include <render/material/Material.h>

MeshObject::MeshObject() : GameObject() {
  _isRenderable = true;
  _cullingData.type = CullingData::Type::OBB;
}

void MeshObject::render(IRenderer &renderer) {
  if (!_mesh || !_material) {
    return;
  }

  RenderOperation rop = _getDefaultRenderOp();
  rop.mesh = _mesh;
  rop.material = _material;
  renderer.addRenderOperation(rop, _renderQueue);
}

void MeshObject::start() {
  if (_mesh) {
    _cullingData.bounds = _mesh->aabb();
  }
}
