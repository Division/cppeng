//
// Created by Sidorenko Nikita on 3/31/18.
//

#ifndef CPPWRAPPER_MATERIALTYPES_H
#define CPPWRAPPER_MATERIALTYPES_H

#include "Material.h"
#include "EngMath.h"
#include <vector>

class MaterialSingleColor: public Material {
public:
  MaterialSingleColor();
  void color(vec4 color) { _bindings.vec4Bindings[_colorBinging].v = color; }
  vec4 color() const { return _bindings.vec4Bindings[_colorBinging].v; }

protected:
  int _colorBinging = -1;
};

class MaterialDebug: public Material {
public:
  MaterialDebug();
};


typedef std::shared_ptr<MaterialSingleColor> MaterialSingleColorPtr;

class MaterialTexture: public Material {
public:
  MaterialTexture();
  void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
  TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }
  void texture1(TexturePtr texture) { return; _bindings.textureBindings[_texture1Binding].texture = texture; }
  TexturePtr texture1() const { return _bindings.textureBindings[_texture1Binding].texture; }

protected:
  int _texture0Binding = -1;
  int _texture1Binding = -1;
};


class MaterialLighting: public Material {
public:
  MaterialLighting();
};


class MaterialTerrain: public Material {
public:
  explicit MaterialTerrain(int layerCount, bool specularmap = false);
  void diffuse(TexturePtr texture, int layer) { _bindings.textureBindings[_diffuseBindings[layer]].texture = texture; }
  TexturePtr diffuse(int layer) const { return _bindings.textureBindings[_diffuseBindings[layer]].texture; }
  void normalMap(TexturePtr texture, int layer) { if (_normalMapBindings.empty()) return; _bindings.textureBindings[_normalMapBindings[layer]].texture = texture; }
  TexturePtr normalMap(int layer) const { return _bindings.textureBindings[_normalMapBindings[layer]].texture; }
  void splatmap(TexturePtr texture) { _bindings.textureBindings[_splatmapBinging].texture = texture; }
  TexturePtr splatmap() const { return _bindings.textureBindings[_splatmapBinging].texture; }
  void specularmap(TexturePtr texture) { _bindings.textureBindings[_specularmapBinging].texture = texture; }
  TexturePtr specularmap() const { return _bindings.textureBindings[_specularmapBinging].texture; }

protected:
  std::vector<int> _diffuseBindings;
  std::vector<int> _normalMapBindings;
  int _splatmapBinging;
  int _specularmapBinging;
};

// This material is used for testing only
// Actual projection rendering is built into the render pipeline
class MaterialTextureProjection: public Material {
public:
  MaterialTextureProjection();
  void projectedTexture(TexturePtr texture) {
    _bindings.textureBindings[_projectedTextureBinding].texture = texture;
  }
  TexturePtr projectedTexture() const { return _bindings.textureBindings[_projectedTextureBinding].texture; }
  void projectedTextureMatrix(mat4 matrix) {
    _bindings.mat4Bindings[_projectedTextureMatrixBinding].matrix = matrix;
  }
  mat4 projectedTextureMatrix() const { return _bindings.mat4Bindings[_projectedTextureMatrixBinding].matrix; }
protected:
  int _projectedTextureBinding = -1;
  int _projectedTextureMatrixBinding = -1;
};


#endif //CPPWRAPPER_MATERIALTYPES_H
