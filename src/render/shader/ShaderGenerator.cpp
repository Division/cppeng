//
// Created by Sidorenko Nikita on 3/23/18.
//

#include "ShaderGenerator.h"
#include "system/Logging.h"
#include <unordered_map>
#include <string>
#include "loader/ShaderLoader.h"
#include "Shader.h"

#ifdef __EMSCRIPTEN__
const std::string GLSL_VERSION = "300 es";
const bool PLATFORM_WEBGL = true;
#else
const std::string GLSL_VERSION = "330";
const bool PLATFORM_WEBGL = false;
#endif

const std::string TEMPLATE_ROOT = "resources/shaderTpl/";

const std::string TEMPLATE_LIST[] = {
    "root", // First goes the root template
    "camera",
    "object_params",
    "vertex_color_fragment_main",
    "vertex_color_fragment",
    "vertex_color_vertex_main",
    "vertex_color_vertex",
    "lighting_fragment_main",
    "lighting_fragment",
    "lighting_vertex_main",
    "lighting_vertex",
    "lighting_process_point",
    "lighting_process_spot",
    "lighting_process_projector",
    "lighting_process_decal",
    "normal_mapping_fragment_main",
    "normal_mapping_fragment",
    "normal_mapping_vertex_main",
    "normal_mapping_vertex",
    "skinning_vertex_main",
    "skinning_vertex",
    "texture_fragment_main",
    "texture_fragment",
    "terrain"
};

const std::map<ShaderCaps, std::string> CAPS_TO_PARAM_MAP = {
    { ShaderCaps::Color, "COLOR" },
    { ShaderCaps::VertexColor, "VERTEX_COLOR" },
    { ShaderCaps::PointSize, "POINT_SIZE" },
    { ShaderCaps::Billboard, "BILLBOARD" },
    { ShaderCaps::Lighting, "LIGHTING" },
    { ShaderCaps::NormalMap, "NORMAL_MAP" },
    { ShaderCaps::SpecularMap, "SPECULAR_MAP" },
    { ShaderCaps::Texture0, "TEXTURE0" },
    { ShaderCaps::TerrainLayer0, "TERRAIN_LAYER0" },
    { ShaderCaps::TerrainLayer1, "TERRAIN_LAYER1" },
    { ShaderCaps::TerrainLayer2, "TERRAIN_LAYER2" },
    { ShaderCaps::ProjectedTexture, "PROJECTED_TEXTURE" },
    { ShaderCaps::Skinning, "SKINNING" },
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
  _env.add_callback(tplName, 1, [&env, name, templateMap](inja::Parsed::Arguments args, json data) {
    auto param = env.get_argument<std::string>(args, 0, data);
    data["MODE"] = param;
    std::string res = env.render_template(templateMap.at(name), data);
    return res;
  });
}

std::string ShaderGenerator::generateShaderSource(ShaderCapsSetPtr caps, const std::string rootTemplate) const {
  json data = this->_getJSONForCaps(caps);
  data["version"] = GLSL_VERSION;
  data["WEBGL"] = PLATFORM_WEBGL;
  data["USE_BUFFER_TEXTURE"] = ENGINE_USE_BUFFER_TEXTURE;

  auto result = _env.render_template(_templateMap.at(rootTemplate), data);

  return result;
}

void ShaderGenerator::setupTemplates () {
  for (auto &filename : TEMPLATE_LIST) {
    auto path = TEMPLATE_ROOT + filename + ".glsl";
    auto tpl = _env.parse_template(path);
    _templateMap[filename] = tpl;
  }
}

void ShaderGenerator::addTemplate(const std::string &name) {
  if (_templateMap.count(name)) { return; }

  auto path = TEMPLATE_ROOT + name;
  _templateMap[name] = _env.parse_template(path);
  _addTemplateCallback(name);
}

json ShaderGenerator::_getJSONForCaps(ShaderCapsSetPtr caps) const {
  json result;

  for (auto cap : CAPS_TO_PARAM_MAP) {
    auto scap = cap.first;
    bool hasCap = caps->hasCap(scap);
    auto key = cap.second;
    result[key] = hasCap;
  }

  return result;
}

ShaderPtr ShaderGenerator::getShaderWithCaps(std::shared_ptr<ShaderCapsSet> caps, const std::string &rootTemplate) const {
  ShaderPtr result;

  auto &shaderCache = _shaders[rootTemplate];
  auto iterator = shaderCache.find(caps->getBitmask());
  if (iterator == shaderCache.end()) {
    std::string shaderSource = this->generateShaderSource(caps, rootTemplate);
    std::stringstream stream;
    stream.str(shaderSource);
    std::string vertexSource;
    std::string fragmentSource;

    loader::loadShader(stream, &vertexSource, &fragmentSource);
    result = std::make_shared<Shader>(vertexSource, fragmentSource);
    shaderCache[caps->getBitmask()] = result;
    result->setupUniformsForCaps(caps);
  } else {
    result = iterator->second;
  }

  return result;
}

ShaderPtr ShaderGenerator::getShaderWithCaps(std::shared_ptr<ShaderCapsSet> caps) const {
  return getShaderWithCaps(caps, ROOT_TEMPLATE);
}
