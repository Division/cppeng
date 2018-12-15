//
// Created by Sidorenko Nikita on 4/6/18.
//

#ifndef CPPWRAPPER_MODELBUNDLE_H
#define CPPWRAPPER_MODELBUNDLE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <istream>
#include "render/mesh/Mesh.h"
#include "EngMath.h"
#include "nlohmann/json.hpp"
#include <system/Logging.h>
#include "SkinningData.h"
//#include "HierarchyData.h"

using namespace nlohmann;

struct AnimationData {
  float duration;
  int fps;
  int frameCount;
  bool hasPosition;
  bool hasRotation;
  bool hasScale;
  bool isMatrix;
  std::string name;

  std::vector<mat4> matrices;
  std::vector<vec3> positions;
  std::vector<quat> rotations;
  std::vector<vec3> scales;

  int stride;

  mat4 getMatrix(int frame) { return matrices[frame]; }
  vec3 getPosition(int frame) { return positions[frame]; }
  quat getRotation(int frame) { return rotations[frame]; }
  vec3 getScale(int frame) { return scales[frame]; }

  mat4 getMatrix(int frame, float time) {
    auto m1 = getMatrix(frame % frameCount);
    auto m2 = getMatrix((frame + 1) % frameCount);
    return m1 + (m2 - m1) * time;
  }
  vec3 getPosition(int frame, float time) { return glm::lerp(getPosition(frame % frameCount), getPosition((frame + 1) % frameCount), time); }
  quat getRotation(int frame, float time) { return glm::slerp(getRotation(frame % frameCount), getRotation((frame + 1) % frameCount), time); }
  vec3 getScale(int frame, float time) { return glm::lerp(getScale(frame % frameCount), getScale((frame + 1) % frameCount), time); }

  void loadFrames(std::vector<float> &frames);
  int getStride() const;
  int getElementCount() const;
  int bytesPerFrame () { return stride * 4; }

  void loadFromJSON(const json &jsonData);
};

class ModelBundle;
typedef std::shared_ptr<ModelBundle> ModelBundlePtr;
typedef std::shared_ptr<AnimationData> AnimationDataPtr;

class ModelBundle {
public:
  explicit ModelBundle(const std::string &url) : _url(url) { ENGLog("ModelBundle create"); }
  ~ModelBundle() { ENGLog("ModelBundle destroy"); }

  const std::string &url() const { return _url; }
  const MeshPtr getMesh(const std::string &name) const;
  const HierarchyData &hierarchy() const { return _hierarchy; }
  const HierarchyData *findHierarchy(std::string name, const HierarchyData *root = nullptr) const;

  const SkinningDataPtr getSkinning(const std::string &name) const { return _skinning.at(name); }
  const SkinningDataPtr getDefaultSkinning() const { return _skinning.begin()->second; }

  void addMesh(std::string name, MeshPtr mesh);
  void addAnimationData(AnimationDataPtr animationData);

  void loadHiererchy(const json &hierarchyData);
  void loadSkinning(const json &skinningData);

private:
  std::string _url; // unique name of the resource
  std::unordered_map<std::string, MeshPtr> _meshes; // map of meshes
  std::unordered_map<std::string, SkinningDataPtr> _skinning; // map of skin data
  std::unordered_map<std::string, AnimationDataPtr> _animations; // map of meshes
  HierarchyData _hierarchy;
};

#endif //CPPWRAPPER_MODELBUNDLE_H
