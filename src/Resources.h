//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_RESOURCES_H
#define CPPWRAPPER_RESOURCES_H

#include <string>
#include <map>
#include <unordered_map>
#include "render/mesh/Mesh.h"
#include "resources/ModelBundle.h"

class Shader;

class Resources {
public:
  static const ModelBundlePtr loadModel(const std::string &filename);
  static const ModelBundlePtr loadModelFromStream(std::istream &stream, const std::string &url);
  static bool loadShader(const std::string &filename);
  static bool loadShaderFromStream(std::istream &stream, const std::string &name);
  static Shader *getShader(const std::string &name);
private:
  static std::map<std::string, Shader *> _shaders;
  static std::unordered_map<std::string, ModelBundlePtr> _models;
};


#endif //CPPWRAPPER_RESOURCES_H
