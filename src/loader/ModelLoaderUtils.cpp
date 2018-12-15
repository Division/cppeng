//
// Created by Sidorenko Nikita on 2018-12-13.
//

#include "ModelLoaderUtils.h"

mat4 loader::getMatrixFromJSON(json matrix) {
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