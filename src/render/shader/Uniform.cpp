//
// Created by Sidorenko Nikita on 3/19/18.
//

#include "Uniform.h"
#include <iostream>

const std::map<UniformType, std::string> UNIFORM_NAMES = {
    { UniformType::ProjectionMatrix, "uPMatrix" },
    { UniformType::ModelViewMatrix, "uMVMatrix" },
    { UniformType::ViewMatrix, "uViewMatrix" },
    { UniformType::Texture0, "uTexture0" },
    { UniformType::Texture1, "uTexture1" },
    { UniformType::NormalMap, "uNormalMap" }
};