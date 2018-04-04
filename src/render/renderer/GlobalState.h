//
// Created by Sidorenko Nikita on 4/3/18.
//

#ifndef CPPWRAPPER_GLOBALSTATE_H
#define CPPWRAPPER_GLOBALSTATE_H

#include "EngMath.h"
#include "render/material/Material.h"

class GlobalState : public Material {
public:
  mat4 viewMatrix;
  mat4 projectionMatrix;
};


#endif //CPPWRAPPER_GLOBALSTATE_H
