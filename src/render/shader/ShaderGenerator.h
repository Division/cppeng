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
  void addTemplate(const std::string &name);
  std::string generateShaderSource(ShaderCapsSetPtr caps, const std::string rootTemplate) const;
  ShaderPtr getShaderWithCaps(std::shared_ptr<ShaderCapsSet> caps) const;
  ShaderPtr getShaderWithCaps(std::shared_ptr<ShaderCapsSet> caps, const std::string &rootTemplate) const;

private:
  mutable inja::Environment _env;
  std::unordered_map<std::string, inja::Template> _templateMap;

private:
  typedef std::unordered_map<ShaderCapsSet::Bitmask, ShaderPtr> ShaderCache;

  mutable std::unordered_map<std::string, ShaderCache> _shaders;
  json _getJSONForCaps(ShaderCapsSetPtr shaderCaps) const;
  void _addTemplateCallback(std::string tplName);
};


#endif //CPPWRAPPER_SHADERGENERATOR_H
