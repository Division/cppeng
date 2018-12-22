//
// Created by Sidorenko Nikita on 4/8/18.
//

#include "HierarchyLoader.h"
#include <system/Logging.h>

GameObjectPtr
loader::loadHierarchy(ModelBundlePtr bundle, const HierarchyDataPtr hierarchyToLoad, const MaterialPicker *materialPicker) {
  auto hierarchy = hierarchyToLoad;
  if (!hierarchy && bundle) {
    hierarchy = bundle->hierarchy();
  }

  GameObjectPtr object;

  if (hierarchy->hasGeometry) {
    MeshObjectPtr meshObject = CreateGameObject<MeshObject>();
    meshObject->mesh(bundle->getMesh(hierarchy->geometry));
    object = meshObject;

    if (materialPicker) {
      meshObject->material(materialPicker->getMaterial(hierarchy));
    } else {
      MaterialPicker picker;
      meshObject->material(picker.getMaterial(hierarchy));
    };
  } else {
    object = CreateGameObject<GameObject>();
  }

  object->transform()->setMatrix(hierarchy->transform);
  object->name(hierarchy->name);
  object->sid(hierarchy->sid);

  if (bundle) {
    auto animationData = bundle->getAnimationData(hierarchy->id);
    if (animationData) {
      object->animation()->animationData(animationData);
    }
  }

//  ENGLog("Added object with name %s", object->name().c_str());

  for (auto &childHierarchy : hierarchy->children) {
    auto child = loadHierarchy(bundle, childHierarchy, materialPicker);
    child->transform()->parent(object->transform());
  }

  return object;
}


SkinnedMeshObjectPtr loadSkinnedMesh(ModelBundlePtr bundle, SkinningDataPtr skinningData = nullptr) {
  return loader::loadSkinnedMesh<SkinnedMeshObject>(bundle, skinningData);
}

MaterialPtr loader::MaterialPicker::getMaterial(const HierarchyDataPtr hierarchy) const {
  return _defaultMaterial;
}

loader::MaterialPicker::MaterialPicker() {
  _defaultMaterial = std::make_shared<MaterialLighting>();
}

loader::MaterialPicker::MaterialPicker(MaterialPtr material) {
  _defaultMaterial = material;
}
