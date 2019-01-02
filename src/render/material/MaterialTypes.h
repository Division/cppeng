//
// Created by Sidorenko Nikita on 3/31/18.
//

#ifndef CPPWRAPPER_MATERIALTYPES_H
#define CPPWRAPPER_MATERIALTYPES_H

#include "Material.h"
#include "EngMath.h"
#include <vector>

// Note: materials should not be instantiated every frame

// TODO: these materials probably should be refactored into a single configurable material class

class MaterialSingleColor: public Material {
public:
  MaterialSingleColor();
  void color(vec4 color) { _bindings.vec4Bindings[_colorBinging].v = color; }
  vec4 color() const { return _bindings.vec4Bindings[_colorBinging].v; }

protected:
  int _colorBinging = -1;
};

class MaterialBillboard: public Material {
public:
  MaterialBillboard();
  void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
  TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }

  void color(vec4 color) { _bindings.vec4Bindings[_colorBinging].v = color; }
  vec4 color() const { return _bindings.vec4Bindings[_colorBinging].v; }

protected:
  int _texture0Binding = -1;
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

protected:
  int _texture0Binding = -1;
};

class MaterialTextureLighting: public Material {
public:
  MaterialTextureLighting();
  void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
  TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }

protected:
  int _texture0Binding = -1;
};

class MaterialTextureSpecularMap: public Material {
public:
  MaterialTextureSpecularMap();
  void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
  TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }
  void specularMap(TexturePtr texture) { _bindings.textureBindings[_specularmapBinding].texture = texture; }
  TexturePtr specularMap() const { return _bindings.textureBindings[_specularmapBinding].texture; }

protected:
  int _texture0Binding = -1;
  int _specularmapBinding = -1;
};

class MaterialTextureBump: public Material {
public:
  MaterialTextureBump();
  void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
  TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }
  void normalMap(TexturePtr texture) { _bindings.textureBindings[_normalmapBinding].texture = texture; }
  TexturePtr normalMap() const { return _bindings.textureBindings[_normalmapBinding].texture; }

protected:
  int _texture0Binding = -1;
  int _normalmapBinding = -1;
};

class MaterialTextureBumpSpecular: public Material {
public:
  MaterialTextureBumpSpecular();
  void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
  TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }
  void normalMap(TexturePtr texture) { _bindings.textureBindings[_normalmapBinding].texture = texture; }
  TexturePtr normalMap() const { return _bindings.textureBindings[_normalmapBinding].texture; }
  void specularMap(TexturePtr texture) { _bindings.textureBindings[_specularmapBinding].texture = texture; }
  TexturePtr specularMap() const { return _bindings.textureBindings[_specularmapBinding].texture; }

protected:
  int _texture0Binding = -1;
  int _normalmapBinding = -1;
  int _specularmapBinding = -1;
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

// NOTE: this material is used for testing only
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
