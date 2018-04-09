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

  mat4 getMatrixFromJSON(json matrix) {
    mat4 result;

    if (matrix.is_array()) {
      int counter = 0;
      for (auto item : matrix) {
        auto number = item.get<float>();
        int column = counter / 4;
        int row = counter % 4;
        result[column][row] = number;
        counter++;
      }
    }

//    ENGLog("JSON Matrix:\n%s\nParsed matrix:\n%s", matrix.dump().c_str(), to_string(result).c_str());

    return result;
  }

}

#endif //CPPWRAPPER_MODELLOADERUTILS_H
