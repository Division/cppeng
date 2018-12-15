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

using namespace nlohmann;

struct HierarchyData {
  std::string name;
  std::string id;
  std::string material;
  std::string geometry;
  bool hasGeometry = false;
  bool hasMaterial = false;
  mat4 transform;
  std::vector<HierarchyData> children;

  void loadFromJSON(const json &jsonData);
};


#endif //CPPWRAPPER_HIERARCHYDATA_H
