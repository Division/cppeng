//
// Created by Sidorenko Nikita on 4/6/18.
//

#include "ModelBundle.h"
#include "system/Logging.h"
#include "loader/ModelLoaderUtils.h"

void AnimationData::loadFromJSON(const json &jsonData) {
  duration = jsonData["duration"].get<float>();
  fps = jsonData["fps"].get<int>();
  frameCount = jsonData["frameCount"].get<int>();
  name = jsonData["name"].get<std::string>();

  hasPosition = jsonData["hasPosition"].get<bool>();
  hasRotation = jsonData["hasRotation"].get<bool>();
  hasScale = jsonData["hasScale"].get<bool>();
  isMatrix = jsonData["isMatrix"].get<bool>();

  stride = getStride();
}

int AnimationData::getStride() const {
  int result = 0;
  if (hasPosition) result += 3;
  if (hasRotation) result += 4;
  if (hasScale) result += 3;
  if (isMatrix) result += 16;

  return result;
}

int AnimationData::getElementCount() const {
  if (isMatrix) {
    return frameCount * 16;
  } else {
    int result = 0;
    if (hasPosition) result += frameCount * 3;
    if (hasRotation) result += frameCount * 4;
    if (hasScale) result += frameCount * 3;
    return result;
  }
}

void AnimationData::loadFrames(std::vector<float> &frames) {
  if (isMatrix) {
    matrices.resize(frameCount);
    memcpy(&matrices[0], &frames[0], sizeof(mat4) * frameCount);
  } else {
    int addedCount = 0;
    if (hasPosition) {
      positions.resize(frameCount);
      memcpy(&positions[0], &frames[addedCount], sizeof(vec3) * frameCount);
      addedCount += sizeof(vec3) * frameCount;
    }
    if (hasRotation) {
      rotations.resize(frameCount);
      memcpy(&rotations[0], &frames[addedCount], sizeof(quat) * frameCount);
      addedCount += sizeof(quat) * frameCount;
    }
    if (hasScale) {
      scales.resize(frameCount);
      memcpy(&scales[0], &frames[addedCount], sizeof(vec3) * frameCount);
    }
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

void ModelBundle::addAnimationData(AnimationDataPtr animationData) {
  _animations[animationData->name] = animationData;
}

void ModelBundle::loadSkinning(const json &skinningData) {
  for (auto it = skinningData.begin(); it != skinningData.end(); it++) {
    auto skin = std::make_shared<SkinningData>();
    skin->loadFromJSON(it.key(), it.value());
    _skinning[it.key()] = skin;
  }
}

const HierarchyData *ModelBundle::findHierarchy(std::string name, const HierarchyData *root) const {
  auto data = root ? root : &_hierarchy;

  if (data->name == name) {
    return data;
  }

  if (data) {
    for (auto &child : data->children) {
      auto result = findHierarchy(name, &child);
      if (result) {
        return result;
      }
    }
  }

  return nullptr;
}
