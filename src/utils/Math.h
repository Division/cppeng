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

struct OBB {
  vec3 position;
  vec3 size;
  quat rotation;

  OBB() = default;
  OBB(const vec3 &position, const vec3 &size, const quat &rotation = quat()) {
    this->position = position;
    this->rotation = rotation;
    this->size = size;
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

  AABB multMatrix(const mat4 &matrix) {
//    vec3 s = size();

//    const vec3 vertices[] = {
//      matrix * min,
//      matrix * vec4((min + s * vec3(0, 0, 1)),
//      matrix * (min + s * vec3(1, 0, 1)),
//      matrix * (min + s * vec3(1, 0, 0)),
//      matrix * (max - s * vec3(1, 0, 1)),
//      matrix * (max - s * vec3(1, 0, 0)),
//      matrix * (max),
//      matrix * (max - s * vec3(0, 0, 1)),
//    };

//    AABB result (vertices[0], vertices[0]);
//    for (int i = 1; i < 8; i++) {
//      result.expand(vertices[i]);
//    }

//    return result;
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

struct Rect {
  float x = 0;
  float y = 0;
  float width = 0;
  float height = 0;

  Rect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
};

#endif //CPPWRAPPER_MATH_H
