//
// Created by Sidorenko Nikita on 7/14/18.
//

#ifndef CPPWRAPPER_SPRITESHEET_H
#define CPPWRAPPER_SPRITESHEET_H

#include <string>
#include "EngMath.h"
#include <vector>
#include "Texture.h"

struct SpriteData {
  std::string filename;
  Rect bounds;
};

class SpriteSheet {
public:
  SpriteSheet();

private:
  float _width;
  float _height;
  std::vector<SpriteData> _sprites;
};


#endif //CPPWRAPPER_SPRITESHEET_H
