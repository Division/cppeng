//
// Created by Sidorenko Nikita on 4/18/18.
//

#ifndef CPPWRAPPER_LIGHTOBJECT_H
#define CPPWRAPPER_LIGHTOBJECT_H

#include "scene/GameObject.h"
#include <memory>
#include "render/shader/UniformBufferStruct.h"
#include "utils/MeshGeneration.h"
#include "render/material/MaterialTypes.h"
#include "EngTypes.h"
#include "render/shading/IShadowCaster.h"

enum class LightObjectType : int {
  Point = 0,
  Spot
};

class LightObject : public GameObject, public IShadowCaster {
public:
  friend class Scene;
  friend class ShadowMap;

  LightObject();

  float radius() const { return _radius; }
  void radius(float value) { _radius = value; _updateAttenuation(); }
  AABB bounds();

  float squareAttenuation() const { return _squareAttenuation; }
  void squareAttenuation(float value) { _squareAttenuation = value; _updateRadius(); }

  float linearAttenuation() const { return _linearAttenuation; }
  void linearAttenuation(float value) { _linearAttenuation = value; _updateRadius(); }

  void attenuation(float linear, float square);

  LightObjectType type() const { return _type; }
  void type(LightObjectType value) { _type = value; }

  vec3 color() const { return _color; }
  void color(vec3 value) { _color = value; }

  float coneAngle() const { return _coneAngle; }
  void coneAngle(float value) { _coneAngle = value; }

  float getSpotRadius(float height);

  bool castShadows() const override { return _castShadows; };
  void castShadows(bool value) { _castShadows = value; }

  unsigned int index() const { return _index; }

  void enableDebug();
  bool debugEnabled() { return _debugMesh && _debugMaterial; }

  UBOStruct::Light getLightStruct() const;

  void render(IRenderer &renderer) override;
  void postUpdate() override;

  // ICameraParamsProvider
  uvec2 cameraViewSize() const override { return  uvec2(_viewport.z, _viewport.w); }
  vec3 cameraPosition() const override { return transform()->worldPosition(); }
  mat4 cameraViewProjectionMatrix() const override { return _projectionMatrix * _viewMatrix; }
  vec3 cameraLeft() const override { return transform()->left(); }
  vec3 cameraRight() const override { return transform()->right(); }
  vec3 cameraUp() const override { return transform()->up(); }
  vec3 cameraDown() const override { return transform()->down(); }
  mat4 cameraViewMatrix() const override { return _viewMatrix; }
  mat4 cameraProjectionMatrix() const override { return _projectionMatrix; }
  vec4 cameraViewport() const override { return _viewport; }
  unsigned int cameraIndex() const override { return _cameraIndex; };
  void cameraIndex(unsigned int index) override { _cameraIndex = index; };

private:
  // Common light properties
  float _radius = 13;
  vec3 _color = vec3(1, 1, 1);
  float _squareAttenuation = 0.44;
  float _linearAttenuation = 0.35;
  float _lightCutoff = 0.001;
  unsigned int _index; // index in scene array

  // Spotlight properties
  float _coneAngle = 30;

  // Shadows
  mat4 _projectionMatrix;
  mat4 _viewMatrix;
  vec4 _viewport;
  float _zMin = 0.1f;
  bool _castShadows = false;
  unsigned int _cameraIndex = 0;

  void _updateAttenuation();
  void _updateRadius();

  LightObjectType _type = LightObjectType::Point;

  MeshPtr _debugMesh;
  MaterialSingleColorPtr _debugMaterial;

protected:
  // To be used by ShadowMap class
  void viewport(vec4 value) override { _viewport = value; };
  vec4 viewport() const override { return _viewport; };
};

typedef std::shared_ptr<LightObject> LightObjectPtr;

#endif //CPPWRAPPER_LIGHTOBJECT_H
