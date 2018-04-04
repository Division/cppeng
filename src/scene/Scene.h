//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_SCENE_H
#define CPPWRAPPER_SCENE_H

#include <vector>
#include <unordered_map>
#include "GameObject.h"
#include "objects/Camera.h"

class Scene : public IGameObjectManager {
public:
  void setAsDefault();
  Scene();

  const std::unordered_map<GameObjectID, GameObjectPtr> *const gameObjectMap() const { return &_objectMap; };
  const std::vector<GameObjectPtr> *const gameObjects() const { return &_gameObjects; }
  // Temporary returns all objects. TODO: add camera parameter
  const std::vector<GameObjectPtr> *const visibleObjects(CameraPtr camera) const { return &_gameObjects; }
  void update(float dt);

  int cameraCount() { return (int)_cameraMap.size(); }
  const std::unordered_map<GameObjectID, CameraPtr> *cameras() const { return &_cameraMap; }

protected:
  // IGameObjectManager
  void addGameObject(GameObjectPtr object) override;
  void destroyGameObject(GameObjectPtr object) override;

  // ITransformManager
  void transformChangeParent(Transform *transform, Transform *oldParent, Transform *newParent) override;

  // update
  void _updateTransforms();

protected:
  std::unordered_map<GameObjectID, GameObjectPtr> _objectMap; // maps GameObject::id() to GameObject

  // TODO: make cameras sorted list
  std::unordered_map<GameObjectID, CameraPtr> _cameraMap; // maps GameObject::id() to Camera
  std::vector<GameObjectPtr> _gameObjects; // Full list of scene game objects
  std::unordered_map<GameObjectID, Transform *>_rootTransformMap; // maps GameObject::id() to the top level transforms

  void _processAddedObject(GameObjectPtr object);
  void _processRemovedObject(GameObjectPtr object);
};


#endif //CPPWRAPPER_SCENE_H
