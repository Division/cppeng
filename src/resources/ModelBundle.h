//
// Created by Sidorenko Nikita on 4/6/18.
//

#ifndef CPPWRAPPER_MODELBUNDLE_H
#define CPPWRAPPER_MODELBUNDLE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <istream>
#include "render/mesh/Mesh.h"
#include "EngMath.h"
#include "nlohmann/json.hpp"
#include <system/Logging.h>

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

class ModelBundle;
typedef std::shared_ptr<ModelBundle> ModelBundlePtr;

class ModelBundle {
public:
  explicit ModelBundle(const std::string &url) : _url(url) { ENGLog("ModelBundle create"); }
  ~ModelBundle() { ENGLog("ModelBundle destroy"); }

  const std::string &url() const { return _url; }
  const MeshPtr getMesh(const std::string &name) const;
  const HierarchyData &hierarchy() const { return _hierarchy; }

  void addMesh(std::string name, MeshPtr mesh);

  void loadHiererchy(const json &hierarchyData);
private:
  std::string _url; // unique name of the resource
  std::unordered_map<std::string, MeshPtr> _meshes; // map of meshes
  HierarchyData _hierarchy;
//  std::unordered_map<std::string, AnimationDataPtr> _animations; // map of meshes
};

#endif //CPPWRAPPER_MODELBUNDLE_H
