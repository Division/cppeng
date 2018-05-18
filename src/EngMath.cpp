//
// Created by Sidorenko Nikita on 4/3/18.
//

#include "EngMath.h"

const vec3 VectorLeft(-1 ,0, 0);
const vec3 VectorRight(1 ,0, 0);
const vec3 VectorUp(0 ,1, 0);
const vec3 VectorDown(0 ,-1, 0);
const vec3 VectorForward(0 ,0, -1);
const vec3 VectorBack(0 ,0, 1);


float glm::dotf(const vec2 &a, const vec3 &b) { return a.x * b.x + a.y * b.y; }
float glm::dotf(const vec3 &a, const vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float glm::dotf(const vec4 &a, const vec4 &b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
