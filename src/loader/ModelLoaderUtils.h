//
// Created by Sidorenko Nikita on 4/7/18.
//

#ifndef CPPWRAPPER_MODELLOADERUTILS_H
#define CPPWRAPPER_MODELLOADERUTILS_H

#include "EngMath.h"
#include "nlohmann/json.hpp"
#include "system/Logging.h"

using namespace nlohmann;

namespace loader {

  mat4 getMatrixFromJSON(json matrix);

}

#endif //CPPWRAPPER_MODELLOADERUTILS_H
