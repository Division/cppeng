//
// Created by Sidorenko Nikita on 2018-12-27.
//

#include "Math.h"
#include "cmath"
#include <glm/glm.hpp>

using namespace glm;

Sphere boundingSphereForFrustum(float width, float height, float zNear, float zFar, float fov) {
  Sphere result;
  float k = sqrtf(1 + height * height / (width * width)) * tanf(fov / 2);
  float squareK = k * k;
  if (squareK >= (zFar - zNear) / (zFar + zNear)) {
    result.position = vec3(0, 0, -zFar);
    result.radius = zFar * k;
  } else {
    result.position = vec3(0, 0, -0.5f * (zNear + zFar) * (1.0f + squareK));
    result.radius = 0.5f * sqrtf(powf(zFar - zNear, 2) + 2.0f * (zFar * zFar + zNear * zNear) * squareK + powf(zFar + zNear, 2) * squareK * squareK);
  }

  return result;
};