//
// Created by Sidorenko Nikita on 3/23/18.
//

#ifndef CPPWRAPPER_SHADERGENERATOR_H
#define CPPWRAPPER_SHADERGENERATOR_H

#include <string>
#include <unordered_map>
#include "Shader.h"

#include "nlohmann/json.hpp"
#include "inja.hpp"

using json = nlohmann::json;

class ShaderGenerator {
public:
  ShaderGenerator();
  void setupTemplates();
  std::string generateShaderSource(ShaderCapsSetPtr caps);

private:
  inja::Environment _env;
  std::unordered_map<std::string, inja::Template> _templateMap;

private:
  json _getJSONForCaps(ShaderCapsSetPtr shaderCaps);

};


#endif //CPPWRAPPER_SHADERGENERATOR_H
