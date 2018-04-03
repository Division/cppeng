//
// Created by Sidorenko Nikita on 3/23/18.
//

#include "ShaderGenerator.h"
#include "system/Logging.h"
#include <unordered_map>
#include <string>

#ifdef __EMSCRIPTEN__
const std::string GLSL_VERSION = "300 es";
const bool PLATFORM_WEBGL = true;
#else
const std::string GLSL_VERSION = "150";
const bool PLATFORM_WEBGL = false;
#endif

const std::string TEMPLATE_ROOT = "resources/shaderTpl/";

const std::string TEMPLATE_LIST[] = {
    "root", // First goes the root template
    "uniforms_vs"
};

const std::map<ShaderCaps, std::string> CAPS_TO_PARAM_MAP = {
    { ShaderCaps::Color, "COLOR" }
};

const auto ROOT_TEMPLATE = TEMPLATE_LIST[0];

ShaderGenerator::ShaderGenerator() {
  this->setupTemplates();

  // Add non-root templates as callbacks so that
  // they can be referenced as {{ template_name }}
  for (auto &filename : TEMPLATE_LIST) {
    if (filename != ROOT_TEMPLATE) {
      _addTemplateCallback(filename);
    }
  }
}

void ShaderGenerator::_addTemplateCallback(std::string tplName) {
  auto env = _env;
  const auto &templateMap = _templateMap;
  auto name = tplName;
  _env.add_callback(tplName, 0, [&env, name, templateMap](inja::Parsed::Arguments args, json data) {
    std::string res = env.render_template(templateMap.at(name), data);
    return res;
  });
}

std::string ShaderGenerator::generateShaderSource(ShaderCapsSetPtr caps) {
  json data = this->_getJSONForCaps(caps);
  data["version"] = GLSL_VERSION;
  data["WEBGL"] = PLATFORM_WEBGL;

  return _env.render_template(_templateMap[ROOT_TEMPLATE], data);
}

void ShaderGenerator::setupTemplates () {
  for (auto &filename : TEMPLATE_LIST) {
    auto path = TEMPLATE_ROOT + filename + ".tpl";
    auto tpl = _env.parse_template(path);
    _templateMap[filename] = tpl;
  }
}

json ShaderGenerator::_getJSONForCaps(ShaderCapsSetPtr caps) {
  json result;

  for (auto cap : caps->caps()) {
    auto scap = (ShaderCaps)cap;
    bool hasCap = CAPS_TO_PARAM_MAP.find((ShaderCaps)cap) != CAPS_TO_PARAM_MAP.end();
    result[CAPS_TO_PARAM_MAP.at(scap)] = hasCap;
  }

//  if (SET_CONTAINS_CAP(caps, ShaderCaps.ColorWrite)) {
//
//  }

  return result;
}
