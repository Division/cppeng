//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_MESHOBJECT_H
#define CPPWRAPPER_MESHOBJECT_H

#include <scene/GameObject.h>
#include <render/mesh/Mesh.h>

class Material;

class MeshObject : public GameObject {
public:
  MeshObject();
protected:
  MeshPtr _mesh;

  void render(IRenderer &renderer) override;
};


#endif //CPPWRAPPER_MESHOBJECT_H
