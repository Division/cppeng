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

enum class LightObjectType : int {
  Point,
  Spot
};

class LightObject : public GameObject {
public:
  friend class Scene;

  LightObject();

  float radius() const { return _radius; }
  void radius(float value) { _radius = value; _updateAttenuation(); }

  float squareAttenuation() const { return _squareAttenuation; }
  void squareAttenuation(float value) { _squareAttenuation = value; _updateRadius(); }

  LightObjectType type() { return _type; }
  void type(LightObjectType value) { _type = value; }

  vec3 color() const { return _color; }
  void color(vec3 value) { _color = value; }

  unsigned int index() const { return _index; }

  void enableDebug();

  UBOStruct::Light getLightStruct() const;

  void render(IRenderer &renderer) override;

private:
  float _radius;
  float _squareAttenuation;
  vec3 _color = vec3(1, 1, 1);
  float _lightCutoff = 0.02;

  void _updateAttenuation();
  void _updateRadius();

  unsigned int _index; // index in scene array
  LightObjectType _type;

  MeshPtr _debugMesh;
  MaterialSingleColorPtr _debugMaterial;

};

typedef std::shared_ptr<LightObject> LightObjectPtr;

#endif //CPPWRAPPER_LIGHTOBJECT_H
