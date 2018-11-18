//
// Created by Sidorenko Nikita on 11/15/18.
//

#ifndef CPPWRAPPER_DECAL_H
#define CPPWRAPPER_DECAL_H

#include "scene/GameObject.h"
#include <memory>
#include "utils/MeshGeneration.h"
#include "render/material/MaterialTypes.h"
#import "EngTypes.h"
#import "EngMath.h"

enum class ProjectorType : int {
  Projection = 0, // Calculates as a light which color is based on the texture
  Decal // substitutes diffuse color and in some cases normals of the rendered object
};

// Object to use for decals and projective light sources rendering
class Projector: public GameObject {
public:
  friend class Scene;

  Projector() = default;

  AABB bounds();

  vec4 color() const { return _color; }
  void color(vec4 value) { _color = value; }

  // counts as fovY
  void fov(float fov) { _fov = fov; }
  float fov() const { return _fov; }

  void zNear(float zNear) { _zNear = zNear; }
  float zNear() const { return _zNear; }

  void zFar(float zFar) { _zFar = zFar; }
  float zFar() const { return _zFar; }

  void texture(TexturePtr texture) { _texture = texture; }
  TexturePtr texture() const { return _texture; }

  void isOrthographic(bool isOrthographic) { _isOrthographic = isOrthographic; }
  bool isOrthographic() const { return _isOrthographic; }

  void orthographicSize(float orthographicSize) { _orthographicSize = orthographicSize; }
  float orthographicSize() const { return _orthographicSize; }

  void adjustAttenuation(float cutoff = 0.02);

  // Frustum matrix
  const mat4 viewProjection() const { return _viewProjection; }

  void getEdgePoints (std::vector<vec3> &outEdgePoints);

  ProjectorType type() const { return _type; }
  void type(ProjectorType value) { _type = value; }

  unsigned int index() const { return _index; }

  void setDebugEnabled(bool enabled) { _debugEnabled = enabled; }

  UBOStruct::Projector getProjectorStruct() const;

  void render(IRenderer &renderer) override;

  void postUpdate() override;

private:
  // common
  vec4 _color = vec4(1, 1, 1, 1);
  unsigned int _index; // index in scene array
  TexturePtr _texture;
  TexturePtr _normalMap;
  mat4 _viewProjection;
  float _aspect = 1;
  float _zNear = 1;
  float _zFar = 10;
  float _fov = 30; // Perspective projectors properties in degrees
  float _squareAttenuation = 1;
  ProjectorType _type = ProjectorType::Decal;

  // Orthographic
  bool _isOrthographic = false;
  float _orthographicSize = 1.0f;

  bool _debugEnabled = false;

private:
  mat4 _getProjection();
};

#endif //CPPWRAPPER_DECAL_H
