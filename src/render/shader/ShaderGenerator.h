//
// Created by Sidorenko Nikita on 3/23/18.
//

#ifndef CPPWRAPPER_SHADERGENERATOR_H
#define CPPWRAPPER_SHADERGENERATOR_H

#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "inja.hpp"
#include "ShaderCaps.h"

using json = nlohmann::json;

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

class ShaderGenerator {
public:
  ShaderGenerator();
  void setupTemplates();
  std::string generateShaderSource(ShaderCapsSetPtr caps) const;
  ShaderPtr getShaderWithCaps(std::shared_ptr<ShaderCapsSet> caps) const;

private:
  mutable inja::Environment _env;
  std::unordered_map<std::string, inja::Template> _templateMap;

private:
  mutable std::unordered_map<ShaderCapsSet::Bitmask, ShaderPtr> _shaders;
  json _getJSONForCaps(ShaderCapsSetPtr shaderCaps) const;
  void _addTemplateCallback(std::string tplName);
};


#endif //CPPWRAPPER_SHADERGENERATOR_H
