//
// Created by Sidorenko Nikita on 3/28/18.
//

#include "Transform.h"
#include "GameObject.h"

void Transform::_updateTransform(const mat4 *parentTransform, bool parentUpdated) {

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

  for (auto childTransform : _children) {
    childTransform->_updateTransform(&_worldMatrix, _dirty || parentUpdated);
  }

  _dirty = false;
}

void Transform::addChild(Transform *transform) {
  transform->setParent(this);
}

void Transform::setParent(Transform *transform) {
  if (this->parent() != transform) {
    _manager->transformChangeParent(this, this->parent(), transform);
    _dirty = true;
  }
}
