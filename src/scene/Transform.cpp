//
// Created by Sidorenko Nikita on 3/28/18.
//

#include "Transform.h"
#include "GameObject.h"

void Transform::_updateTransform(const mat4 *parentTransform, bool parentUpdated, bool skipChildren) {

  // Update local transform if needed
  if (_dirty) {
    _localMatrix = mat4();
    _localMatrix = glm::translate(_localMatrix, _position);
    _localMatrix *= mat4_cast(_rotation);
    _localMatrix = glm::scale(_localMatrix, _scale);
  }

  if (parentUpdated || _dirty) {
    if (parentTransform) {
      _worldMatrix = *parentTransform * _localMatrix;
    } else {
      _worldMatrix = _localMatrix;
    }
  }

  if (!skipChildren) {
    for (auto &childTransform : _children) {
      childTransform->_updateTransform(&_worldMatrix, _dirty || parentUpdated);
    }
  }

  _dirty = false;
}

void Transform::_addChild(Transform *child) {
  bool found = false;
  for (auto &transform : _children) {
    if (transform->gameObject()->id() == child->gameObject()->id()) {
      found = true;
      break;
    }
  }

  if (!found) {
    _children.push_back(child);
  }
}

void Transform::_removeChild(Transform *child) {
  for (int i = 0; i < _children.size(); i++) {
    if (_children[i]->gameObject()->id() == child->gameObject()->id()) {
      // Remove i-th element:
      // Replace i-th element with the last and pop array
      _children[i] = _children.back();
      _children.pop_back();
    }
  }
}

void Transform::setParent(Transform *transform) {
  if (this->parent() != transform) {
    _manager->transformChangeParent(this, this->parent(), transform);
    _dirty = true;
  }
}

void Transform::rotate(vec3 axis, float angle) {
  _rotation = glm::rotate(_rotation, angle, axis);
  setDirty();
}
