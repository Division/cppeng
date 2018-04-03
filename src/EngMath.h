//
// Created by Sidorenko Nikita on 3/28/18.
//

#ifndef CPPWRAPPER_MATH_H
#define CPPWRAPPER_MATH_H

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI M_PI
#define RAD(value) (value * (float)PI / 180)
#define DEG(value) (value / (float)PI * 180)

using namespace glm;

extern const vec3 VectorLeft;
extern const vec3 VectorRight;
extern const vec3 VectorUp;
extern const vec3 VectorDown;
extern const vec3 VectorForward;
extern const vec3 VectorBack;

#endif //CPPWRAPPER_MATH_H
