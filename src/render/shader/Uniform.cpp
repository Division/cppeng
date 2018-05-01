//
// Created by Sidorenko Nikita on 3/19/18.
//

#include "Uniform.h"
#include <iostream>

const std::map<UniformName, std::string> UNIFORM_NAMES = {
    { UniformName::Color, "uColor" },
    { UniformName::ProjectionMatrix, "uPMatrix" },
    { UniformName::ModelViewMatrix, "uMVMatrix" },
    { UniformName::ViewMatrix, "uViewMatrix" },
    { UniformName::NormalMatrix, "uNormalMatrix" },
    { UniformName::Texture0, "uTexture0" },
    { UniformName::Texture1, "uTexture1" },
    { UniformName::NormalMap, "uNormalMap" },
    { UniformName::LightGrid, "uLightGrid" },
    { UniformName::LightIndices, "uLightIndices" }
};

const std::map<UniformName, unsigned int> UNIFORM_TEXTURE_BLOCKS = {
    { UniformName::Texture0, 0 },
    { UniformName::Texture1, 1 },
    { UniformName::NormalMap, 2 },
    { UniformName::LightGrid, 3 },
    { UniformName::LightIndices, 4 }
};

const std::map<UniformBlockName, std::string> UNIFORM_BLOCK_NAMES = {
    { UniformBlockName::Transform, "TransformBlock" },
    { UniformBlockName::Light, "LightBlock" },
    { UniformBlockName::Camera, "CameraBlock" }
};