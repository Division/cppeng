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

void GameObject::render(IRenderer &renderer) {

}
