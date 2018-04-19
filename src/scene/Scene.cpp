//
// Created by Sidorenko Nikita on 3/28/18.
//

#include "Scene.h"

#define IS_CAMERA(object) (bool)(dynamic_cast<Camera *>((object).get()))
#define IS_LIGHT(object) (bool)(dynamic_cast<LightObject *>((object).get()))

void Scene::setAsDefault() {
  GameObject::_defaultManager = this;
}

Scene::Scene() {
  if (!GameObject::_defaultManager) {
    setAsDefault();
  }
}

void Scene::addGameObject(GameObjectPtr object) {
  if (_objectMap.find(object->id()) == _objectMap.end()) {
    _gameObjects.push_back(object);
    _objectMap[object->id()] = object;

    object->_setManager(this);
    this->transformChangeParent(object->transform(), nullptr, nullptr);

    this->_processAddedObject(object);
  }
}

void Scene::_processAddedObject(GameObjectPtr object) {
  // Object is camera
  if (IS_CAMERA(object)) {
    CameraPtr camera = std::dynamic_pointer_cast<Camera>(object);
    _cameraMap[object->id()] = camera;
  }

  // Object is light
  else if (IS_LIGHT(object)) {
    LightObjectPtr light = std::dynamic_pointer_cast<LightObject>(object);
    _lights.push_back(light); // TODO: try to find free empty index
    light->_index = _lights.size() - 1;
    _lightCount += 1;
  }
}

void Scene::_processRemovedObject(GameObjectPtr object) {
  // Object is camera
  if (IS_CAMERA(object)) {
    _cameraMap.erase(object->id());
  }

  else if (IS_LIGHT(object)) {
    for (auto &light : _lights) {
      if (light->id() == object->id()) {
        light = nullptr; // just set to null for future reuse
        _lightCount -= 1;
        break;
      }
    }
  }

  object->_destroy();
}

// TODO: destroy out of update loop
void Scene::destroyGameObject(GameObjectPtr object) {
  if (object->destroyed()) {
    ENGLog("ERROR: attempt to destroy already destroyed object");
    return;
  }

  if (_objectMap.find(object->id()) != _objectMap.end()) {
    _objectMap.erase(object->id());
    object->transform()->setParent(nullptr);

    auto foundObject = std::find_if(_gameObjects.begin(), _gameObjects.end(), [&object](const GameObjectPtr &obj) -> bool {
      return object->id() == obj->id();
    });

    if (foundObject == _gameObjects.end()) {
      std::string error = "ERROR destroying object, not found in the list";
      ENGLog(error.c_str());
      throw error;
    }

    auto index = foundObject - _gameObjects.begin();
    _gameObjects[index] = _gameObjects[_gameObjects.size() - 1];
    _gameObjects.pop_back();

    _processRemovedObject(object);
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
  for (auto &object : _gameObjects) {
    if (object->active()) {
      object->update(dt);
    }
  }

  _updateTransforms();

  for (auto &object : _gameObjects) {
    if (object->active()) {
      object->postUpdate();
    }
  }
}

void Scene::_updateTransforms() {
  for (auto &transform : _rootTransformMap) {
    transform.second->_updateTransform(nullptr, false);
  }
}
