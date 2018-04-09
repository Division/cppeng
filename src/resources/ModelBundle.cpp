//
// Created by Sidorenko Nikita on 4/6/18.
//

#include "ModelBundle.h"
#include "system/Logging.h"
#include "loader/ModelLoaderUtils.h"

void HierarchyData::loadFromJSON(const json &jsonData) {
  name = jsonData["name"];
  id = jsonData["id"];

  hasGeometry = jsonData.find("geometry") != jsonData.end() && !jsonData["geometry"].empty();
  hasMaterial = jsonData.find("material") != jsonData.end() && !jsonData["material"].empty();

  geometry = hasGeometry ? jsonData["geometry"] : "";
  material = hasMaterial ? jsonData["material"] : "";

  ENGLog("%s(%s), geom: %s, material: %s", name.c_str(), id.c_str(), geometry.c_str(), material.c_str());

  transform = loader::getMatrixFromJSON(jsonData["transform"]);

  // No children found
  if (jsonData.find("children") == jsonData.end() || !jsonData["children"].is_array()) {
    return;
  }

  // Children found, load them
  auto jsonChildren = jsonData["children"];
  int size = jsonChildren.size();
  children.resize(size);
  for (int i = 0; i < size; i++) {
    children[i].loadFromJSON(jsonChildren.at(i));
  }
}


const MeshPtr ModelBundle::getMesh(const std::string &name) const {
  MeshPtr result;

  if (_meshes.find(name) != _meshes.end()) {
    result = _meshes.at(name);
  } else {
    result = std::make_shared<Mesh>();
    ENGLog("Can't find bundle '%s' in url %s", name.c_str(), _url.c_str());
  }

  return result;
}

void ModelBundle::loadHiererchy(const json &hierarchyData) {
  _hierarchy.loadFromJSON(hierarchyData);
}

void ModelBundle::addMesh(std::string name, MeshPtr mesh) {
  _meshes[name] = mesh;
}
