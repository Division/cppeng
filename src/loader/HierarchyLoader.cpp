//
// Created by Sidorenko Nikita on 4/8/18.
//

#include "HierarchyLoader.h"
#include <system/Logging.h>

GameObjectPtr loader::loadHierarchy(ModelBundlePtr bundle, const HierarchyData *hierarchy) {
  if (!hierarchy) {
    hierarchy = &bundle->hierarchy();
  }

  GameObjectPtr object;

  if (hierarchy->hasGeometry) {
    MeshObjectPtr meshObject = CreateGameObject<MeshObject>();
    meshObject->mesh(bundle->getMesh(hierarchy->geometry));
    object = meshObject;

    MaterialPicker picker;
    meshObject->material(picker.getMaterial(hierarchy));
  } else {
    object = CreateGameObject<GameObject>();
  }

  object->transform()->setMatrix(hierarchy->transform);
  object->name(hierarchy->name);

//  ENGLog("Added object with name %s", object->name().c_str());

  for (auto &childHierarchy : hierarchy->children) {
    auto child = loadHierarchy(bundle, &childHierarchy);
    child->transform()->parent(object->transform());
  }

  return object;
}

MaterialPtr loader::MaterialPicker::getMaterial(const HierarchyData *hierarchy) {
  MaterialPtr result;

  if (hierarchy->hasMaterial) {
    //
//    result = std::make_shared<MaterialSingleColor>();
  } else {
//    result = std::make_shared<MaterialSingleColor>();
    result = std::make_shared<MaterialLighting>();
  }

  return result;
}
