//
// Created by Sidorenko Nikita on 2018-12-13.
//

#ifndef CPPWRAPPER_HIERARCHYDATA_H
#define CPPWRAPPER_HIERARCHYDATA_H

#include <string>
#include <vector>
#include "EngMath.h"
#include "nlohmann/json.hpp"
#include "HierarchyData.h"
#include <memory>
#include <functional>

using namespace nlohmann;

struct MaterialData {
  std::shared_ptr<std::string> diffuse;
  unsigned char diffuseUV = 0;
};

struct HierarchyData {
  std::string name;
  std::string id;
  std::string sid;
  std::string geometry;
  std::string light;
  std::string originalNodeID;
  bool hasGeometry = false;
  std::shared_ptr<MaterialData> material;
  bool isLight = false;
  mat4 transform;
  std::vector<std::shared_ptr<HierarchyData>> children;
  void forEachChild(bool recursive, std::function<void(std::shared_ptr<HierarchyData>)> callback) const;

  void loadFromJSON(const json &jsonData);
};

typedef std::shared_ptr<HierarchyData> HierarchyDataPtr;

#endif //CPPWRAPPER_HIERARCHYDATA_H
