//
// Created by Sidorenko Nikita on 3/28/18.
//

#include "GameObject.h"

GameObjectID GameObject::instanceCounter = 0;
IGameObjectManager *GameObject::_defaultManager = nullptr;

GameObject::GameObject() : _transform(this) {
  _id = ++instanceCounter;
}

void GameObject::_setManager(IGameObjectManager *manager) {
  _manager = manager;
  _transform._manager = manager;
}

void GameObject::update(float dt) {

}

void GameObject::postUpdate() {

}

void GameObject::render(IRenderer &renderer) {

}

void GameObject::_destroy() {
  // Also destroy children
  for (auto &transform : _transform._children) {
    transform->parent(nullptr);
    GameObjectPtr child(transform->gameObject());
    DestroyGameObject(child);
  }

  _destroyed = true;
  _active = false;
  _transform._parent = nullptr;
  _manager = nullptr;

  _transform._children.clear();
}

void DestroyGameObject(GameObjectPtr object) {
  if (object->_manager) {
    object->_manager->destroyGameObject(object);
  } else {
    ENGLog("Error: can't _destroy object with null _manager");
  }
}
