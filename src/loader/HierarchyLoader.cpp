//
// Created by Sidorenko Nikita on 4/8/18.
//

#include "HierarchyLoader.h"
#include <system/Logging.h>

GameObjectPtr
loader::loadHierarchy(ModelBundlePtr bundle, const HierarchyData *hierarchy, const MaterialPicker *materialPicker) {
  if (!hierarchy && bundle) {
    hierarchy = &bundle->hierarchy();
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

  if (bundle) {
    auto animationData = bundle->getAnimationData(object->name());
    if (animationData) {
      object->animation()->animationData(animationData);
    }
  }

//  ENGLog("Added object with name %s", object->name().c_str());

  for (auto &childHierarchy : hierarchy->children) {
    auto child = loadHierarchy(bundle, &childHierarchy, materialPicker);
    child->transform()->parent(object->transform());
  }

  return object;
}

SkinnedMeshObjectPtr loader::loadSkinnedMesh(ModelBundlePtr bundle, SkinningDataPtr skinningData) {
  auto skinning = skinningData ? skinningData : bundle->getDefaultSkinning();
  auto skinnedMeshObject = CreateGameObject<SkinnedMeshObject>();

  auto hierarchy = bundle->findHierarchy(skinning->name);
  if (!hierarchy) {
    throw std::runtime_error("Skinned mesh hierarchy not found");
  }

  skinnedMeshObject->transform()->setMatrix(hierarchy->transform);
  skinnedMeshObject->setSkinningData(bundle, skinning);
  skinnedMeshObject->mesh(bundle->getMesh(hierarchy->geometry));
  skinnedMeshObject->material(std::make_shared<MaterialLighting>());

  return skinnedMeshObject;
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
