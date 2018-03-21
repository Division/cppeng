//
// Created by Sidorenko Nikita on 3/19/18.
//

#include "Uniform.h"
#include <iostream>

const std::map<UniformType, std::string> UNIFORM_NAMES = {
    { UniformType::ProjectionMatrix, "uPMatrix" },
    { UniformType::ModelViewMatrix, "uMVMatrix" },
    { UniformType::ViewMatrix, "uViewMatrix" }
};