//
// Created by Sidorenko Nikita on 2018-12-15.
//

#ifndef CPPWRAPPER_ANIMATIONCONTROLLER_H
#define CPPWRAPPER_ANIMATIONCONTROLLER_H

#include "resources/ModelBundle.h"
#include <unordered_map>
#include <string>
#include <vector>

class GameObject;

struct AnimationSequence {
  int startFrame = 0;
  int count = 0;
};

class AnimationController {
public:
  template <typename T> friend std::shared_ptr<T> CreateGameObject();

  AnimationController() = default;
  AnimationController(const AnimationController &a) = delete;

  void animationData(AnimationDataPtr value);
  const AnimationDataPtr animationData() const { return _animationData; }
  bool hasAnimation() const { return (bool)_animationData; }
  bool autoUpdate() const { return _autoUpdate; }
  bool autoUpdate(bool value) { _autoUpdate = value; }

  std::shared_ptr<GameObject> gameObject() { return _gameObject.lock(); }
  void addChildController(std::shared_ptr<AnimationController> controller) { _childControllers.push_back(controller); }

  void play(std::string animationName = "default", bool loop = false);
  void update(float dt);

  void applyAnimationFrame(int frame1, int frame2, float delta);

private:
  AnimationSequence _currentSequence;

  float _animationTime = 0;
  float _timePerFrame = 0;
  float _currentFrame = 0;
  bool _isLooping = false;
  bool _isPlaying = false;

  AnimationDataPtr _animationData;
  std::weak_ptr<GameObject> _gameObject;
  bool _autoUpdate = true;

  // list of controllers that have the same animations set
  // They are controlled manually from a single place (this, parent one)
  std::vector<std::shared_ptr<AnimationController>> _childControllers;
  std::unordered_map<std::string, AnimationSequence> _sequences;
};

typedef std::shared_ptr<AnimationController> AnimationControllerPtr;

#endif //CPPWRAPPER_ANIMATIONCONTROLLER_H
