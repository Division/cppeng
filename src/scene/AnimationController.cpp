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
  int currentFrameInt = (int)lround(floorf(_currentFrame)) % _currentSequence.count;
  int frame1 = (int)(_currentSequence.startFrame + currentFrameInt) % _animationData->frameCount;
  int frame2 = (int)(_currentSequence.startFrame + currentFrameInt) % _animationData->frameCount;
  float delta = _currentFrame - floorf(_currentFrame);

  applyAnimationFrame(frame1, frame2, delta);

  for (auto &child : _childControllers) {
    if (child->animationData()) {
      child->applyAnimationFrame(frame1, frame2, delta);
    }
  }
}

void AnimationController::play(std::string animationName, bool loop) {
  if (!this->_animationData) {
    ENGLog("Attempt to play with null AnimationData");
    return;
  }
  _currentSequence = _sequences.at(animationName);
  _isPlaying = true;
  _isLooping = loop;
  _animationTime = 0;
}

void AnimationController::animationData(AnimationDataPtr value) {
  _animationData = value;
  _timePerFrame = 1.0f / _animationData->fps;
  _sequences.clear();

  if (!value->sequences.empty()) {
    _sequences["default"] = value->sequences[0];

    for (auto &sequence : value->sequences) {
      _sequences[sequence.name] = sequence;
    }
  } else {
    _sequences["default"] = { "default", 0, _animationData->frameCount };
  }
}

void AnimationController::applyAnimationFrame(int frame1, int frame2, float delta) {
  auto obj = gameObject();

  if (_animationData->isMatrix) {
    obj->transform()->setMatrix(_animationData->getMatrix(frame1, frame2, delta));
  } else {
    obj->transform()->position(_animationData->getPosition(frame1, frame2, delta));
    if (_animationData->hasScale) {
      obj->transform()->scale(_animationData->getScale(frame1, frame2, delta));
    }
    obj->transform()->rotation(_animationData->getRotation(frame1, frame2, delta));
  }

}
