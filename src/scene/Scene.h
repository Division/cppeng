//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_SCENE_H
#define CPPWRAPPER_SCENE_H

#include <vector>
#include <unordered_map>
#include "GameObject.h"

class Scene : public IGameObjectManager {
public:
  void setAsDefault();
  Scene();

protected:
  // IGameObjectManager
  void addGameObject(GameObjectPtr object) override;
  void addGameObject(GameObject *object) override;
  void removeGameObject(GameObject *object) override;

  // ITransformManager
  void transformChangeParent(Transform *transform, Transform *oldParent, Transform *newParent) override;

private:
  std::unordered_map<GameObjectID, GameObjectPtr> _objectMap; // maps GameObject::id() to GameObject
  std::vector<GameObjectPtr> _gameObjects;
};


#endif //CPPWRAPPER_SCENE_H
