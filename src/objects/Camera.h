//
// Created by Sidorenko Nikita on 4/3/18.
//

#ifndef CPPWRAPPER_CAMERA_H
#define CPPWRAPPER_CAMERA_H

#include "scene/GameObject.h"

class Camera : public GameObject {
public:

private:
  float _fow;
  float _aspect; // screen aspect
  unsigned long _mask = ~0ul; // mask to filter gameObjects
};

typedef std::shared_ptr<Camera> CameraPtr;


#endif //CPPWRAPPER_CAMERA_H
