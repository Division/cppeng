//
// Created by Sidorenko Nikita on 11/20/18.
//

#ifndef CPPWRAPPER_VIEW_H
#define CPPWRAPPER_VIEW_H

#include <memory>

#include "IRenderer.h"
#include "ICameraParamsProvider.h"
#include "EngMath.h"

// Wraps a single pass with a particular camera
class RenderPass {
public:
  uvec2 size () const { return this->_camera ? this->_camera->cameraViewSize() : uvec2(0, 0); }

  std::shared_ptr<ICameraParamsProvider> camera () const { return _camera; }
  void camera(std::shared_ptr<ICameraParamsProvider> value) { _camera = value; }

  RenderMode mode () const { return _mode; }
  void mode (RenderMode value) { _mode = value; }

  bool clearDepth = true;
  bool clearColor = true;

private:
  RenderMode _mode = RenderMode::Normal;
  std::shared_ptr<ICameraParamsProvider> _camera;
};


#endif //CPPWRAPPER_VIEW_H
