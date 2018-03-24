//
// Created by Sidorenko Nikita on 3/23/18.
//

#include "ShaderGenerator.h"


#ifdef __EMSCRIPTEN__
const std::string GLSL_VERSION = "300 es";
const bool PLATFORM_WEBGL = true;
#else
const std::string GLSL_VERSION = "150";
const bool PLATFORM_WEBGL = false;
#endif

const std::string TEMPLATE_ROOT = "resources/shaderTpl/";

const std::string TEMPLATE_LIST[] = {
    "root.shader" // First goes the root template
};

ShaderGenerator::ShaderGenerator() {
  this->setupTemplates();
}

std::string ShaderGenerator::generateShaderSource(ShaderCapsSetPtr caps) {
  json data = this->_getJSONForCaps(caps);
  data["version"] = GLSL_VERSION;
  data["WEBGL"] = PLATFORM_WEBGL;

  return _env.render_template(_templateMap[TEMPLATE_LIST[0]], data);
}

void ShaderGenerator::setupTemplates () {
  for (auto filename : TEMPLATE_LIST) {
    auto path = TEMPLATE_ROOT + filename;
    _templateMap[filename] = _env.parse_template(path);
  }
}

json ShaderGenerator::_getJSONForCaps(ShaderCapsSetPtr caps) {
  json result;

//  if (SET_CONTAINS_CAP(caps, ShaderCaps.ColorWrite)) {
//
//  }

  return result;
}
