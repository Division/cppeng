//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_SCENE_H
#define CPPWRAPPER_SCENE_H

#include <vector>
#include <unordered_map>
#include "EngTypes.h"
#include "GameObject.h"

class Scene : public IGameObjectManager {
public:
  void setAsDefault();
  Scene();

  const std::unordered_map<GameObjectID, GameObjectPtr> *const gameObjectMap() const { return &_objectMap; };
  const std::vector<GameObjectPtr> *const gameObjects() const { return &_gameObjects; }
  // Temporary returns all objects.
  const std::vector<ProjectorPtr> *const visibleProjectors(const CameraPtr camera) const { return &_projectors; }
  const std::vector<GameObjectPtr> *const visibleObjects(const CameraPtr camera) const { return &_gameObjects; }
  const std::vector<LightObjectPtr> *const visibleLights(const CameraPtr camera) const { return &_lights; }
  void update(float dt);

  int cameraCount() { return (int)_cameraMap.size(); }
  const std::unordered_map<GameObjectID, CameraPtr> *cameras() const { return &_cameraMap; }

protected:
  // IGameObjectManager
  void addGameObject(GameObjectPtr object) override;
  void destroyGameObject(GameObjectPtr object) override;

  // ITransformManager
  // TODO: change to TransformPtr
  void transformChangeParent(Transform *transform, Transform *oldParent, Transform *newParent) override;

  // update
  void _updateTransforms();

protected:
  unsigned int _lightCount = 0;

  std::unordered_map<GameObjectID, GameObjectPtr> _objectMap; // maps GameObject::id() to GameObject

  // TODO: make cameras sorted list
  std::unordered_map<GameObjectID, CameraPtr> _cameraMap; // maps GameObject::id() to Camera
  std::vector<ProjectorPtr> _projectors; // Array of projectors
  std::vector<LightObjectPtr> _lights; // Array of scene lights
  std::vector<GameObjectPtr> _gameObjects; // Full list of scene game objects
  std::unordered_map<GameObjectID, Transform *>_rootTransformMap; // maps GameObject::id() to the top level transforms

  void _processAddedObject(GameObjectPtr object);
  void _processRemovedObject(GameObjectPtr object);
};

#endif //CPPWRAPPER_SCENE_H
