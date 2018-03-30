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

  const std::vector<GameObjectPtr> &gameObjects() const { return _gameObjects; }

  // Temporary returns all objects. TODO: add camera parameter
  const std::vector<GameObjectPtr> &visibleObjects() const { return _gameObjects; }
  void update(float dt);

protected:
  // IGameObjectManager
  void addGameObject(GameObjectPtr object) override;
  void addGameObject(GameObject *object) override;
  void removeGameObject(GameObject *object) override;

  // ITransformManager
  void transformChangeParent(Transform *transform, Transform *oldParent, Transform *newParent) override;

  // update
  void _updateTransforms();

protected:
  std::unordered_map<GameObjectID, GameObjectPtr> _objectMap; // maps GameObject::id() to GameObject
  std::vector<GameObjectPtr> _gameObjects; // Full list of scene game objects
  std::unordered_map<GameObjectID, Transform *>_rootTransformMap; // maps GameObject::id() to the top level transforms
};


#endif //CPPWRAPPER_SCENE_H
