//
// Created by Sidorenko Nikita on 4/5/18.
//

#include <vector>
#include <string>
#include "ModelLoader.h"
#include "system/Logging.h"
#include "resources/ModelBundle.h"
#include "nlohmann/json.hpp"
#include "EngineGL.h"
#include "system/utils.h"

using namespace nlohmann;

const auto ATTRIB_POSITION = "POSITION";
const auto ATTRIB_NORMAL = "NORMAL";
const auto ATTRIB_TEXCOORD0 = "TEXCOORD0";
const auto ATTRIB_WEIGHT = "WEIGHT";

const std::map<std::string, int> ATTRIB_COUNT = {
  { ATTRIB_POSITION, 3 },
  { ATTRIB_NORMAL, 3 },
  { ATTRIB_TEXCOORD0, 2 },
  { ATTRIB_WEIGHT, 6 }
};

void loadGeometry(std::istream &stream, json geometryJson, ModelBundlePtr bundle);
void loadAnimation(std::istream &stream, json animationJson, ModelBundlePtr bundle);
template <typename T> void loadArray(std::istream &stream, std::vector<T> &data, int count);

ModelBundlePtr loader::loadModel(std::istream &stream, const std::string &url) {
  ModelBundlePtr bundle = std::make_shared<ModelBundle>(url);

  uint16 headerSize = 0;
  stream.read((char *)&headerSize, sizeof(headerSize));

  headerSize = swap_endian<uint16>(headerSize);
//  std::vector<char> headerChars;
//  headerChars.resize(headerSize + 1);
//  headerChars[headerSize] = 0;
//  stream.read(&headerChars[0], headerSize);
//  std::string headerString(&headerChars[0]);

  std::string headerString;
  headerString.resize(headerSize);
  stream.read(&headerString[0], headerSize);


//  ENGLog("JSON: \n%i\n %s", headerString.length(), headerString.c_str());
  json header = json::parse(headerString);
//  ENGLog("JSON:\n%s", header.dump(2).c_str());

  if (header.find("hierarchy") != header.end()) {
    bundle->loadHiererchy(header["hierarchy"]);
  }

  if (header.find("geometry") != header.end()) {
    loadGeometry(stream, header["geometry"], bundle);
  }

  if (header.find("animation") != header.end()) {
    loadAnimation(stream, header["animation"], bundle);
  }

  return bundle;
}

void loadGeometry(std::istream &stream, json geometryJson, ModelBundlePtr bundle) {
  std::vector<float> attribData;
  std::vector<unsigned short> indices;
  for (auto &geom : geometryJson) {
    MeshPtr mesh = std::make_shared<Mesh>(true);
    int indexCount = geom["indexCount"];
    int vertexCount = geom["vertexCount"];
    auto attributes = geom["attributes"];
    std::string name = geom["name"];

    indices.resize(indexCount);
    loadArray<unsigned short>(stream, indices, indexCount);
    mesh->setIndices(indices);

    for (auto &attribID : attributes) {

      auto totalAttribCount = ATTRIB_COUNT.at(attribID) * vertexCount;
      attribData.resize(totalAttribCount);
      loadArray<float>(stream, attribData, totalAttribCount);

      if (attribID == ATTRIB_POSITION) {
        mesh->setVertices(&attribData[0], vertexCount);
      }

      if (attribID == ATTRIB_NORMAL) {
//        mesh->setNormals(&attribData[0], vertexCount);
      }

      if (attribID == ATTRIB_TEXCOORD0) {
        mesh->setTexCoord0(&attribData[0], vertexCount);
      }

      if (attribID == ATTRIB_WEIGHT) {
        mesh->setWeights(&attribData[0], vertexCount);
      }
    }

    if (!mesh->hasNormals()) {
      mesh->calculateNormals();
    }

    if (geom.find("caps") != geom.end()) {
      auto caps = geom["caps"];
      if (caps.find("bump") != caps.end()) {
        mesh->calculateTBN();
      }
    }

    mesh->createBuffer();
    bundle->addMesh(name, mesh);
  }
}

void loadAnimation(std::istream &stream, json animationJson, ModelBundlePtr bundle) {
  if (animationJson.find("skinning") != animationJson.end()) {
    bundle->loadSkinning(animationJson["skinning"]);
  }

  std::vector<float> tempAnimations;

  if (animationJson.find("objects") != animationJson.end()) {
    auto &animatedObjects = animationJson["objects"];
    auto size = animatedObjects.size();

    for (int i = 0; i < size; i++) {
      auto animationData = std::make_shared<AnimationData>();
      animationData->loadFromJSON(animatedObjects.at(i));
      bundle->addAnimationData(animationData);

      tempAnimations.resize((unsigned long)animationData->getElementCount());
      loadArray<float>(stream, tempAnimations, (int)tempAnimations.size());
      animationData->loadFrames(tempAnimations);
    }
  }
}

template <typename T> void loadArray(std::istream &stream, std::vector<T> &data, int count) {
  stream.read((char *)&data[0], sizeof(T) * count);
  for (int i = 0; i < count; i++) {
    data[i] = swap_endian<T>(data[i]);
  }
}
