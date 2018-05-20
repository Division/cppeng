//
// Created by Sidorenko Nikita on 4/8/18.
//

#include "HierarchyLoader.h"
#include <system/Logging.h>

GameObjectPtr
loader::loadHierarchy(ModelBundlePtr bundle, const HierarchyData *hierarchy, const MaterialPicker *materialPicker) {
  if (!hierarchy) {
    hierarchy = &bundle->hierarchy();
  }

  GameObjectPtr object;

  if (hierarchy->hasGeometry) {
    MeshObjectPtr meshObject = CreateGameObject<MeshObject>();
    meshObject->mesh(bundle->getMesh(hierarchy->geometry));
    object = meshObject;

    if (materialPicker) {
      meshObject->material(materialPicker->getMaterial(hierarchy));
    } else{
      MaterialPicker picker;
      meshObject->material(picker.getMaterial(hierarchy));
    };
  } else {
    object = CreateGameObject<GameObject>();
  }

  object->transform()->setMatrix(hierarchy->transform);
  object->name(hierarchy->name);

//  ENGLog("Added object with name %s", object->name().c_str());

  for (auto &childHierarchy : hierarchy->children) {
    auto child = loadHierarchy(bundle, &childHierarchy, materialPicker);
    child->transform()->parent(object->transform());
  }

  return object;
}

MaterialPtr loader::MaterialPicker::getMaterial(const HierarchyData *hierarchy) const {
  return _defaultMaterial;
}

loader::MaterialPicker::MaterialPicker() {
  _defaultMaterial = std::make_shared<MaterialLighting>();
}

loader::MaterialPicker::MaterialPicker(MaterialPtr material) {
  _defaultMaterial = material;
}
