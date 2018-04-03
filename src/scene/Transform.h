//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_TRANSFORM_H
#define CPPWRAPPER_TRANSFORM_H

#include "EngMath.h"
#include <vector>

class GameObject;
class Transform;

class ITransformManager {
public:
  virtual void transformChangeParent(Transform *transform, Transform *oldParent, Transform *newParent) = 0;
};

class Transform {
public:
  friend class GameObject;
  friend class Scene;

  explicit Transform(GameObject *object) : _gameObject(object) {};

  GameObject *gameObject() { return _gameObject; }
  Transform *parent() { return _parent; }
  void parent(Transform *transform) { setParent(transform); };
  void setParent(Transform *transform);

  const vec3 &position() { return _position; }
  void position(const vec3 &position) { _position = position; setDirty(); }

  const quat &rotation() { return _rotation; }
  void rotation(const quat &rotation) { _rotation = rotation; setDirty(); }

  const vec3 &scale() { return _scale; }
  void scale(const vec3 &scale) { _scale = scale; setDirty(); }

  const mat4 &worldMatrix() { return _worldMatrix; }

  void setDirty() { _dirty = true; }

  const std::vector<Transform *> * const children() const { return &_children; }

  void setPosition(const vec3 &position) { _position = position; setDirty(); }
  void setRotation(const quat &rotation) { _rotation = rotation; setDirty(); }
  void setScale(const vec3 &scale) { _scale = scale; setDirty(); }
  void rotate(vec3 axis, float angle);

private:
  vec3 _position = vec3(0, 0, 0);
  quat _rotation;
  vec3 _scale = vec3(1, 1, 1);
  mat4 _localMatrix;
  mat4 _worldMatrix;

  GameObject *_gameObject;
  Transform *_parent = nullptr;
  std::vector<Transform *> _children;
  ITransformManager *_manager = nullptr;

  bool _dirty = true;

private:
  void _updateTransform(const mat4 *parentTransform, bool forceUpdate);

  void _removeChild(Transform *child);
  void _addChild(Transform *child);
};


#endif //CPPWRAPPER_TRANSFORM_H
