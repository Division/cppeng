//
// Created by Sidorenko Nikita on 4/12/18.
//

#ifndef CPPWRAPPER_UNIFORMBUFFERSTRUCT_H
#define CPPWRAPPER_UNIFORMBUFFERSTRUCT_H

namespace UBOStruct {

  struct TransformStruct {
    mat4 transform; // offset = 0, size = 64
    mat4 normalMatrix; // offset = 64, alignment = 16, size = 64
  };

  struct Light {
    vec3 position; // offset = 0, size = 12
    float attenuation = 0; // offset = 12, alignment = 4, size = 4
    vec3 color; // offset = 16, alignment = 16, size = 12
    float padding1 = 0;
    vec3 direction;
    float coneAngle = 0;
  };

  struct Camera {
    vec3 position; // offset = 0, size = 12
    float padding1;
    uvec2 screenSize;
    vec2 padding23;
    mat4 viewMatrix;
    mat4 projectionMatrix;
  };

}


#endif //CPPWRAPPER_UNIFORMBUFFERSTRUCT_H
