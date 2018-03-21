//
// Created by Sidorenko Nikita on 1/21/18.
//

#include "Resources.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <render/shader/Shader.h>
#include <Logging.h>
#include <loader/ShaderLoader.h>

std::map<std::string, Shader *> Resources::_shaders;

bool Resources::loadShader(const std::string &filename) {
  std::ifstream stream;
  stream.open(filename, std::ios::in);
  return Resources::loadShaderFromStream(stream, filename);
}

bool Resources::loadShaderFromStream(std::istream &stream, const std::string &name) {
  bool result;

  try {
    stream.exceptions(std::ios::badbit);

    std::stringstream fileSourceStream;

    char buffer[64];
    do {
      stream.read(buffer, sizeof(buffer));
      long readCount = stream.gcount();
      fileSourceStream.write(buffer, readCount);
    } while (!stream.fail());

    std::string vertexSource;
    std::string fragmentSource;

    loader::loadShader(fileSourceStream, &vertexSource, &fragmentSource);

    Shader *shader = new Shader(vertexSource, fragmentSource);

    Resources::_shaders[name] = shader;
    result = true;

    ENGLog("[Resource] shader loaded %s", name.c_str());
  } catch(std::exception e) {
    result = false;
    ENGLog("Error loading shader %s", name.c_str());
  }

  return result;
}

Shader *Resources::getShader(const std::string &name) {
  if (_shaders.count(name)) {
    return _shaders[name];
  } else {
    return nullptr;
  }
}
