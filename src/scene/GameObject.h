//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_GAMEOBJECT_H
#define CPPWRAPPER_GAMEOBJECT_H

#include <memory>
#include "Transform.h"

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

  GameObject();

  int id() { return _id; }
  Transform *transform() { return &_transform; }
  virtual void update(float dt);

protected:
  Transform _transform;
  IGameObjectManager *_manager;
  void _setManager (IGameObjectManager *manager);
  GameObjectID _id;

private:
  static IGameObjectManager *_defaultManager;
  static int instanceCounter;
};


#endif //CPPWRAPPER_GAMEOBJECT_H
