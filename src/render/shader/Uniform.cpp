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
    { UniformName::TerrainDiffuse0, "uTerrainDiffuse0" },
    { UniformName::TerrainDiffuse1, "uTerrainDiffuse1" },
    { UniformName::TerrainDiffuse2, "uTerrainDiffuse2" },
    { UniformName::TerrainNormal0, "uTerrainNormal0" },
    { UniformName::TerrainNormal1, "uTerrainNormal1" },
    { UniformName::TerrainNormal2, "uTerrainNormal2" },
    { UniformName::TerrainSplatmap, "uTerrainSplatmap" },
    { UniformName::NormalMap, "uNormalMap" },
    { UniformName::SpecularMap, "uSpecularMap" },
    { UniformName::ShadowMap, "uShadowMap" },
    { UniformName::ProjectorTexture, "uProjectorTexture" },
    { UniformName::LightGrid, "uLightGrid" },
    { UniformName::LightIndices, "uLightIndices" },
    { UniformName::ProjectedTexture, "uProjectedTexture" },
    { UniformName::ProjectedTextureMatrix, "uProjectedTextureMatrix" },
    { UniformName::NearFar, "uNearFar" }
};

const std::map<UniformName, unsigned int> UNIFORM_TEXTURE_BLOCKS = {
    { UniformName::Texture0, 0 },
    { UniformName::Texture1, 1 },
    { UniformName::NormalMap, 2 },
    { UniformName::LightGrid, 3 },
    { UniformName::LightIndices, 4 },
    { UniformName::ProjectedTexture, 6 },
    { UniformName::TerrainDiffuse0, 0 },
    { UniformName::TerrainDiffuse1, 1 },
    { UniformName::TerrainDiffuse2, 2 },
    { UniformName::SpecularMap, 5 },
    { UniformName::ShadowMap, 12 },
    { UniformName::TerrainNormal0, 6 },
    { UniformName::TerrainNormal1, 7 },
    { UniformName::TerrainNormal2, 8 },
    { UniformName::TerrainSplatmap, 9 },
    { UniformName::ProjectorTexture, 11 },
};

const std::map<UniformBlockName, std::string> UNIFORM_BLOCK_NAMES = {
    { UniformBlockName::Transform, "TransformBlock" },
    { UniformBlockName::Light, "LightBlock" },
    { UniformBlockName::Camera, "CameraBlock" },
    { UniformBlockName::Projector, "ProjectorBlock" }
};