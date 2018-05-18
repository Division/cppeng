//
// Created by Sidorenko Nikita on 4/21/18.
//

#ifndef CPPWRAPPER_ENGTYPES_H
#define CPPWRAPPER_ENGTYPES_H

class LightObject;
typedef std::shared_ptr<LightObject> LightObjectPtr;

class Camera;
typedef std::shared_ptr<Camera> CameraPtr;

//class MaterialTexutre;
//typedef std::shared_ptr<MaterialTexutre> MaterialTexutrePtr;

class Terrain;
typedef std::shared_ptr<Terrain> TerrainPtr;

#endif //CPPWRAPPER_ENGTYPES_H

