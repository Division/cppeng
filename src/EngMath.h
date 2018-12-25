//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_ENGMATH_H
#define CPPWRAPPER_ENGMATH_H

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/norm.hpp>
#include "utils/Math.h"

#define PI M_PI
#define RAD(value) (value * (float)PI / 180)
#define DEG(value) (value / (float)PI * 180)
#define M_TO_STRING(value) to_string(value)

using namespace glm;

extern const vec3 VectorLeft;
extern const vec3 VectorRight;
extern const vec3 VectorUp;
extern const vec3 VectorDown;
extern const vec3 VectorForward;
extern const vec3 VectorBack;

namespace glm {
  float dotf(const vec2 &a, const vec3 &b);
  float dotf(const vec3 &a, const vec3 &b);
  float dotf(const vec4 &a, const vec4 &b);
}

#endif //CPPWRAPPER_ENGMATH_H
