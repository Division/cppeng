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
#import "EngTypes.h"

enum class LightObjectType : int {
  Point = 0,
  Spot
};

class LightObject : public GameObject {
public:
  friend class Scene;

  LightObject();

  float radius() const { return _radius; }
  void radius(float value) { _radius = value; _updateAttenuation(); }
  AABB bounds();

  float squareAttenuation() const { return _squareAttenuation; }
  void squareAttenuation(float value) { _squareAttenuation = value; _updateRadius(); }

  LightObjectType type() const { return _type; }
  void type(LightObjectType value) { _type = value; }

  vec3 color() const { return _color; }
  void color(vec3 value) { _color = value; }

  float coneAngle() const { return _coneAngle; }
  void coneAngle(float value) { _coneAngle = value; }

  float getSpotRadius(float height);

  unsigned int index() const { return _index; }

  void enableDebug();
  bool debugEnabled() { return _debugMesh && _debugMaterial; }

  UBOStruct::Light getLightStruct() const;

  void render(IRenderer &renderer) override;

private:

  // Common light properties
  float _radius;
  vec3 _color = vec3(1, 1, 1);
  float _squareAttenuation;
  float _lightCutoff = 0.02;
  unsigned int _index; // index in scene array

  // Spotlight properties
  float _coneAngle = 30;

  void _updateAttenuation();
  void _updateRadius();

  LightObjectType _type = LightObjectType::Point;

  MeshPtr _debugMesh;
  MaterialSingleColorPtr _debugMaterial;
};

typedef std::shared_ptr<LightObject> LightObjectPtr;

#endif //CPPWRAPPER_LIGHTOBJECT_H
