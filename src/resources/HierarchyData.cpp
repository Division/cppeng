//
// Created by Sidorenko Nikita on 2018-12-13.
//

#include "HierarchyData.h"
#include "system/Logging.h"
#include "loader/ModelLoaderUtils.h"

void HierarchyData::loadFromJSON(const json &jsonData) {
  name = jsonData["name"];
  id = jsonData["id"];
  if (jsonData.find("sid") != jsonData.end()) {
    sid = jsonData["sid"];
  }

  hasGeometry = jsonData.find("geometry") != jsonData.end() && !jsonData["geometry"].empty();
  isLight = jsonData.find("light") != jsonData.end() && !jsonData["light"].empty();
//  hasMaterial = jsonData.find("material") != jsonData.end() && !jsonData["material"].empty();
  hasMaterial = false; // ignore material for now

  geometry = hasGeometry ? jsonData["geometry"] : "";
//  material = hasMaterial ? jsonData["material"] : "";
  light = isLight ? jsonData["light"] : "";

  transform = loader::getMatrixFromJSON(jsonData["transform"]);

  if (jsonData.find("originalNodeID") != jsonData.end()) {
    originalNodeID = jsonData["originalNodeID"];
  }

  // No children found
  if (jsonData.find("children") == jsonData.end() || !jsonData["children"].is_array()) {
    return;
  }

  // Children found, load them
  auto jsonChildren = jsonData["children"];
  int size = jsonChildren.size();
  children.resize(size);
  for (int i = 0; i < size; i++) {
    children[i] = std::make_shared<HierarchyData>();
    children[i]->loadFromJSON(jsonChildren.at(i));
  }
}

void HierarchyData::forEachChild(bool recursive, std::function<void(std::shared_ptr<HierarchyData>)> callback) const {
  for (auto &child : children) {
    callback(child);
  }

  if (recursive) {
    for (auto &child : children) {
      child->forEachChild(true, callback);
    }
  }
}
