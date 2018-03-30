//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_GAMEOBJECT_H
#define CPPWRAPPER_GAMEOBJECT_H

#include <memory>
#include "Transform.h"
#include "render/renderer/IRenderer.h"

class GameObject;
typedef std::shared_ptr<GameObject> GameObjectPtr;

typedef int GameObjectID;

class IGameObjectManager : public ITransformManager {
public:
  virtual void addGameObject(GameObjectPtr addGameObject) = 0;
  virtual void addGameObject(GameObject *addGameObject) = 0;
  virtual void removeGameObject(GameObject *addGameObject) = 0;
};

class GameObject {
public:
  friend class Scene;

  template <typename T> friend std::shared_ptr<T> CreateGameObject();

  virtual ~GameObject() = default;

  int id() { return _id; }
  bool active() { return _active; }

  Transform *transform() { return &_transform; }

  virtual void update(float dt);
  virtual void render(IRenderer &renderer);

protected:
  GameObject();

  bool _active = true;
  Transform _transform;
  IGameObjectManager *_manager;
  void _setManager (IGameObjectManager *manager);
  GameObjectID _id;

private:
  static IGameObjectManager *_defaultManager;
  static int instanceCounter;
};

template <typename T>
std::shared_ptr<T> CreateGameObject() {
  std::shared_ptr<T> object = std::shared_ptr<T>(new T());

  if (GameObject::_defaultManager) {
    object->_setManager(GameObject::_defaultManager);
    GameObject::_defaultManager->addGameObject(object);
  }

  return object;
}


#endif //CPPWRAPPER_GAMEOBJECT_H
