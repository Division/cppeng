//
// Created by Sidorenko Nikita on 2018-12-13.
//

#ifndef CPPWRAPPER_SKINNEDMESHOBJECT_H
#define CPPWRAPPER_SKINNEDMESHOBJECT_H

#include "MeshObject.h"
#include "resources/SkinningData.h"
#include <unordered_map>
#include <string>
#include <vector>

class SkinnedMeshObject : public MeshObject {
public:
  SkinnedMeshObject() = default;

  void skinningData(SkinningDataPtr skinningData);
  SkinningDataPtr skinningData() const { return _skinningData; };

protected:
  GameObjectPtr _rootJoint = nullptr;
  SkinningDataPtr _skinningData;
  std::unordered_map<std::string, GameObjectPtr> _jointMap;
  std::vector<GameObjectPtr> _jointList;
};


#endif //CPPWRAPPER_SKINNEDMESHOBJECT_H
