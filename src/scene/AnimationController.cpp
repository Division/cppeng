//
// Created by Sidorenko Nikita on 2018-12-15.
//

#include "AnimationController.h"
#include "GameObject.h"
#include <cmath>

void AnimationController::update(float dt) {
  if (!_isPlaying) { return; }

  _animationTime += dt;

  if (!_isLooping && _animationTime >= _animationData->duration) {
    _isPlaying = false;
    _animationTime = _animationData->duration;
  }

  _currentFrame = _animationTime / _timePerFrame;
  int frame1 = (int)(_currentSequence.startFrame + lround(floorf(_currentFrame)) % _currentSequence.count) % _animationData->frameCount;
  int frame2 = (int)(_currentSequence.startFrame + lround(floorf(_currentFrame + 1)) % _currentSequence.count) % _animationData->frameCount;
  float delta = _currentFrame - floorf(_currentFrame);

  applyAnimationFrame(frame1, frame2, delta);

  for (auto &child : _childControllers) {
    if (child->animationData()) {
      child->applyAnimationFrame(frame1, frame2, delta);
    }
  }
}

void AnimationController::play(std::string animationName, bool loop) {
  _currentSequence = _sequences.at(animationName);
  _isPlaying = true;
  _isLooping = loop;
  _animationTime = 0;
}

void AnimationController::animationData(AnimationDataPtr value) {
  _animationData = value;
  _timePerFrame = 1.0f / _animationData->fps;
  _sequences["default"] = { 0, _animationData->frameCount };
}

void AnimationController::applyAnimationFrame(int frame1, int frame2, float delta) {
  auto obj = gameObject();
  obj->transform()->position(_animationData->getPosition(frame1, frame2, delta));
  obj->transform()->scale(_animationData->getScale(frame1, frame2, delta));
  obj->transform()->rotation(_animationData->getRotation(frame1, frame2, delta));
}
