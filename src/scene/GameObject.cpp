//
// Created by Sidorenko Nikita on 3/28/18.
//

#include "GameObject.h"

GameObjectID GameObject::instanceCounter = 0;

GameObject::GameObject() {
  _id = ++instanceCounter;

  if (_defaultManager) {
    _defaultManager->addGameObject(this);
  }
}

void GameObject::_setManager(IGameObjectManager *manager) {
  _manager = manager;
  _transform._manager = manager;
}

void GameObject::update(float dt) {

}
