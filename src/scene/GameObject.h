//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_GAMEOBJECT_H
#define CPPWRAPPER_GAMEOBJECT_H

#include <memory>
#include "Transform.h"
#include "render/renderer/IRenderer.h"
#include "system/Logging.h"
#include <string>

class GameObject;
typedef std::shared_ptr<GameObject> GameObjectPtr;

typedef int GameObjectID;

class IGameObjectManager : public ITransformManager {
public:
  virtual void addGameObject(GameObjectPtr addGameObject) = 0;
  virtual void destroyGameObject(GameObjectPtr addGameObject) = 0;
};

class GameObject {
public:
  friend class Scene;
  template <typename T> friend std::shared_ptr<T> CreateGameObject();
  friend void DestroyGameObject(GameObjectPtr object);

  virtual ~GameObject() = default;

  std::string name() const { return _name; }
  void name(const std::string &name) { _name = name; }

  int id() const { return _id; }
  bool active() const { return _active; }
  bool destroyed() const { return _destroyed; }

  Transform *transform() { return &_transform; }

  virtual void update(float dt);
  virtual void render(IRenderer &renderer);
  virtual void postUpdate(); // called after update() is executed on all scene objects and transforms are updated

protected:
  GameObject();

  std::string _name;

  bool _active = true;
  bool _destroyed = false;
  Transform _transform;
  IGameObjectManager *_manager;
  void _setManager (IGameObjectManager *manager);
  GameObjectID _id;

private:
  static IGameObjectManager *_defaultManager;
  static int instanceCounter;

  void _destroy();
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

extern void DestroyGameObject(GameObjectPtr object);

#endif //CPPWRAPPER_GAMEOBJECT_H
