//
// Created by Sidorenko Nikita on 3/30/18.
//

#include "MeshObject.h"
//#include <render/material/Material.h>

MeshObject::MeshObject() : GameObject() {
  _isRenderable = true;
  _cullingData.type = CullingData::Type::OBB;
}

void MeshObject::update(float dt) {
  _cullingData.bounds = _mesh->aabb();
}

void MeshObject::render(IRenderer &renderer) {
  if (!_mesh || !_material) {
    return;
  }

  RenderOperation rop;
  rop.mesh = _mesh;
  rop.material = _material;
  rop.modelMatrix = transform()->worldMatrix();
  rop.renderOrder = renderOrder;
  rop.debugInfo = name();
  renderer.addRenderOperation(rop, _renderQueue);
}