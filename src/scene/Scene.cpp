//
// Created by Sidorenko Nikita on 3/28/18.
//

#include "Scene.h"

void Scene::setAsDefault() {
  GameObject::_defaultManager = this;
}

Scene::Scene() {
  if (!GameObject::_defaultManager) {
    setAsDefault();
  }
}

void Scene::addGameObject(GameObjectPtr object) {
  addGameObject(object.get());
}

void Scene::addGameObject(GameObject *object) {
  GameObjectPtr objectPtr(object);

  if (_objectMap.find(objectPtr->id()) == _objectMap.end()) {
    _gameObjects.push_back(objectPtr);
    _objectMap[objectPtr->id()] = objectPtr;
    objectPtr->_setManager(this);
  }
}

void Scene::removeGameObject(GameObject *object) {
  if (_objectMap.find(object->id()) == _objectMap.end()) {
    object->transform()->setParent(nullptr);
  }

}

void Scene::transformChangeParent(Transform *transform, Transform *oldParent, Transform *newParent) {
  if (oldParent) {
    oldParent->_removeChild(transform);
  }

  if (newParent) {
    newParent->_addChild(transform);
    _rootTransformMap.erase(transform->gameObject()->id());
  } else {
    _rootTransformMap[transform->gameObject()->id()] = transform;
  }

  transform->_parent = newParent;
}

void Scene::update(float dt) {

}
