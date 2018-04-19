//
// Created by Sidorenko Nikita on 4/12/18.
//

#ifndef CPPWRAPPER_UNIFORMBUFFERSTRUCT_H
#define CPPWRAPPER_UNIFORMBUFFERSTRUCT_H

namespace UBOStruct {

  struct TransformStruct {
    mat4 transform;
    mat4 normalMatrix;
  };

  struct Light {
    vec3 position;
    float padding1;
    float attenuation;
    vec3 color;
  };

}


#endif //CPPWRAPPER_UNIFORMBUFFERSTRUCT_H
