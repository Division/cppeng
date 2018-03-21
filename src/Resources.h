//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_RESOURCES_H
#define CPPWRAPPER_RESOURCES_H

#include <string>
#include <map>

class Shader;

class Resources {
public:
  static bool loadShader(const std::string &filename);
  static bool loadShaderFromStream(std::istream &stream, const std::string &name);
  static Shader *getShader(const std::string &name);
private:
  static std::map<std::string, Shader *> _shaders;
};


#endif //CPPWRAPPER_RESOURCES_H
