//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_MESHOBJECT_H
#define CPPWRAPPER_MESHOBJECT_H

#include <scene/GameObject.h>
#include <render/mesh/Mesh.h>
#include "render/material/Material.h"

class MeshObject : public GameObject {
public:
  MeshObject();
  MaterialPtr material() { return _material; }
  MeshPtr mesh() { return _mesh; }
protected:
  MeshPtr _mesh;
  MaterialPtr _material;

  void render(IRenderer &renderer) override;
};


#endif //CPPWRAPPER_MESHOBJECT_H
