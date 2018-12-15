//
// Created by Sidorenko Nikita on 2018-12-13.
//

#include "SkinnedMeshObject.h"
#include "loader/HierarchyLoader.h"

void SkinnedMeshObject::skinningData(SkinningDataPtr skinningData) {
  if (_rootJoint) {
    DestroyGameObject(_rootJoint);
    _rootJoint = nullptr;
  }

  _skinningData = skinningData;

  _jointList.clear();
  _jointMap.clear();

//  auto recursiveLoop = [&](GameObject *root) {
//    _jointMap[root->name()] = root;
//    for (auto &child : *root->transform()->children()) {
//      recursiveLoop(*child->gameObject());
//    }
//  };

  if (_skinningData) {
    _rootJoint = loader::loadHierarchy(nullptr, &_skinningData->joints);
    _rootJoint->transform()->parent(transform());

  }
}
