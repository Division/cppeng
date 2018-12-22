//
// Created by Sidorenko Nikita on 2018-12-13.
//

#ifndef CPPWRAPPER_SKINNINGDATA_H
#define CPPWRAPPER_SKINNINGDATA_H

#include <string>
#include <vector>
#include "EngMath.h"
#include "nlohmann/json.hpp"
#include "HierarchyData.h"

using namespace nlohmann;

struct SkinningData {
  std::string name;
  std::vector<std::string> jointNames;
  std::vector<mat4> bindPoses;
  HierarchyDataPtr joints;

  void loadFromJSON(const std::string &name, const json &jsonData);
};

typedef std::shared_ptr<SkinningData> SkinningDataPtr;

#endif //CPPWRAPPER_SKINNINGDATA_H
