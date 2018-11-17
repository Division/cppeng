//
// Created by Sidorenko Nikita on 4/21/18.
//

#ifndef CPPWRAPPER_ENGTYPES_H
#define CPPWRAPPER_ENGTYPES_H

#include <memory>

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class LightObject;
typedef std::shared_ptr<LightObject> LightObjectPtr;

class Projector;
typedef std::shared_ptr<Projector> ProjectorPtr;

class Camera;
typedef std::shared_ptr<Camera> CameraPtr;

//class MaterialTexutre;
//typedef std::shared_ptr<MaterialTexutre> MaterialTexutrePtr;

class MaterialTextureProjection;
typedef std::shared_ptr<MaterialTextureProjection> MaterialTextureProjectionPtr;

class Terrain;
typedef std::shared_ptr<Terrain> TerrainPtr;

class SpriteSheet;
typedef std::shared_ptr<SpriteSheet> SpriteSheetPtr;

#endif //CPPWRAPPER_ENGTYPES_H

