//
// Created by Sidorenko Nikita on 4/22/18.
//

#ifndef CPPWRAPPER_MATH_H
#define CPPWRAPPER_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

using namespace glm;

struct Sphere {
  vec3 position;
  float radius;
};

struct OBB {
  mat4 matrix;
  vec3 min = vec3(-1);
  vec3 max = vec3(1);

  OBB() = default;
  OBB(const mat4 &matrix, const vec3 &min, const vec3 &max) {
    this->min = min;
    this->max = max;
    this->matrix = matrix;
  }
};

struct AABB {
  vec3 min;
  vec3 max;

  AABB() = default;

  AABB(const vec3 &vmin, const vec3 &vmax) {
    min = vmin;
    max = vmax;
  }

  vec3 size() { return max - min; }

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

  AABB project(const mat4 &modelMatrix, const mat4 &projectionMatrix, const vec4 &viewport) {
    vec3 s = size();

    const vec3 vertices[] = {
        glm::project(min, modelMatrix, projectionMatrix, viewport),
        glm::project(min + s * vec3(0, 0, 1), modelMatrix, projectionMatrix, viewport),
        glm::project(min + s * vec3(1, 0, 1), modelMatrix, projectionMatrix, viewport),
        glm::project(min + s * vec3(1, 0, 0), modelMatrix, projectionMatrix, viewport),
        glm::project(max - s * vec3(1, 0, 1), modelMatrix, projectionMatrix, viewport),
        glm::project(max - s * vec3(1, 0, 0), modelMatrix, projectionMatrix, viewport),
        glm::project(max, modelMatrix, projectionMatrix, viewport),
        glm::project(max - s * vec3(0, 0, 1), modelMatrix, projectionMatrix, viewport),
    };

    AABB result (vertices[0], vertices[0]);
    for (int i = 1; i < 8; i++) {
      result.expand(vertices[i]);
    }

    return result;
  }
};

Sphere boundingSphereForFrustum(float width, float height, float zNear, float zFar, float fov);

struct Rect {
  float x = 0;
  float y = 0;
  float width = 0;
  float height = 0;

  Rect() = default;
  Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
  operator vec4() const { return vec4(x, y, width, height); }
};

#endif //CPPWRAPPER_MATH_H
