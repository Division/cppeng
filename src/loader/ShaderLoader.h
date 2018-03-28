//
// Created by Sidorenko Nikita on 1/27/18.
//

#ifndef CPPWRAPPER_SHADERLOADER_JS_H
#define CPPWRAPPER_SHADERLOADER_JS_H

#include <string>
#include <ios>

namespace loader {

  void loadShader(std::stringstream &sourceStream, std::string *outVertexSource, std::string *outFragmentSource);

}

#endif //CPPWRAPPER_SHADERLOADER_JS_H
