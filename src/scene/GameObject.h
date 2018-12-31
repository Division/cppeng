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
#include "AnimationController.h"

class GameObject;
typedef std::shared_ptr<GameObject> GameObjectPtr;

typedef int GameObjectID;

struct CullingData {
  enum class Type : int {
    Sphere,
    OBB
  };

  CullingData::Type type;
  Sphere sphere;
  AABB bounds;
};

class IGameObjectManager : public ITransformManager {
public:
  virtual void addGameObject(GameObjectPtr addGameObject) = 0;
  virtual void destroyGameObject(GameObjectPtr addGameObject) = 0;
};

class GameObject {
protected:
  GameObject();

public:
  GameObject(const GameObject &g) = delete;

  friend class Scene;
  template <typename T> friend std::shared_ptr<T> CreateGameObject();
  friend void DestroyGameObject(GameObjectPtr object);

  virtual ~GameObject() = default;

  std::string name() const { return _name; }
  void name(const std::string &name) { _name = name; }

  int id() const { return _id; }

  void layer(unsigned int value) { _layer = value; }
  unsigned int layer() const { return _layer; }

  void sid(const std::string &sid) { _sid = sid; }
  std::string sid() const { return _sid; }

  bool isRenderable() const { return _isRenderable; }
  bool active() const { return _active; }
  void active(const bool active) { _active = active; }
  bool destroyed() const { return _destroyed; }

  AnimationControllerPtr animation() { return _animation; }
  const AnimationControllerPtr animation() const { return _animation; }

  const TransformPtr transform() const { return _transform; }
  TransformPtr transform() { return _transform; }

  const CullingData &cullingData() const { return _cullingData; }

  virtual void start(); // called only once just before the first update()
  virtual void update(float dt);
  virtual void render(IRenderer &renderer);
  virtual void postUpdate(); // called after update() is executed on all scene objects and transforms are updated

protected:
  virtual void _processAnimations(float dt); // called after update, but before postUpdate and transforms calculation
  RenderOperation _getDefaultRenderOp() const { RenderOperation result; result.layer = layer(); return result; };

  std::string _name;
  std::string _sid; // sid of the HierarchyData

  unsigned int _layer = 1 << 0; // default layer is 1

  AnimationControllerPtr _animation;
  CullingData _cullingData;
  bool _isRenderable = false;
  bool _active = true;
  bool _destroyed = false;
  TransformPtr _transform;
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
  auto object = std::shared_ptr<T>(new T());
  object->_transform = std::shared_ptr<Transform>(new Transform(object));
  object->_animation->_gameObject = object;

  if (GameObject::_defaultManager) {
    object->_setManager(GameObject::_defaultManager);
    GameObject::_defaultManager->addGameObject(object);
  }

  return object;
}

extern void DestroyGameObject(GameObjectPtr object);

#endif //CPPWRAPPER_GAMEOBJECT_H
