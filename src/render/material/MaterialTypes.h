//
// Created by Sidorenko Nikita on 3/31/18.
//

#ifndef CPPWRAPPER_MATERIALTYPES_H
#define CPPWRAPPER_MATERIALTYPES_H

#include "Material.h"
#include "EngMath.h"

class MaterialSingleColor: public Material {
public:
  MaterialSingleColor();
  void color(vec4 color) { _bindings.vec4Bindings[_colorBinging].v = color; }
  vec4 color() const { return _bindings.vec4Bindings[_colorBinging].v; }

  int _colorBinging = -1;
};


#endif //CPPWRAPPER_MATERIALTYPES_H