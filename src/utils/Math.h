//
// Created by Sidorenko Nikita on 4/22/18.
//

#ifndef CPPWRAPPER_MATH_H
#define CPPWRAPPER_MATH_H

using namespace glm;

struct Sphere {
  vec3 position;
  float radius;
};

struct AABB {
  vec3 min;
  vec3 max;

  AABB() = default;

  AABB(const vec3 &vmin, const vec3 &vmax) {
    min = vmin;
    max = vmax;
  }

  void expand(const vec3 &point) {
    min = glm::min(point, min);
    max = glm::max(point, max);
  }

  static AABB fromSphere(const vec3 &position, float radius) {
    vec3 radiusVec = vec3(radius, radius, radius);
    return AABB(position - radiusVec, position + radiusVec);
  }

  bool intersectsAABB(const AABB &other) {
    if (other.min.x > max.x || other.max.x < min.x ||
        other.min.y > max.y || other.max.y < min.y ||
        other.min.z > max.z || other.max.z < min.z) {
      return false;
    }

    return true;
  }

  AABB multMatrix(const mat4 &projectionMatrix) {
    AABB result;

    vec4 projMin = projectionMatrix * vec4(min, 1);
    result.min = vec3(projMin) / projMin.w;
    vec4 projMax = projectionMatrix * vec4(max, 1);
    result.max = vec3(projMax) / projMax.w;

    return result;
  }

  AABB project(const mat4 &modelMatrix, const mat4 &projectionMatrix, const vec4 &viewport) {
    AABB result;

    result.min = glm::project(min, modelMatrix, projectionMatrix, viewport);
    result.max = glm::project(max, modelMatrix, projectionMatrix, viewport);

    return result;
  }
};

#endif //CPPWRAPPER_MATH_H
