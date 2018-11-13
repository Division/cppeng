//
// Created by Sidorenko Nikita on 7/14/18.
//

#include "SpritesheetLoader.h"
#include <fstream>
#include <sstream>
#include "render/texture/SpriteSheet.h"
#include "memory"
#include "system/Logging.h"

SpriteSheetPtr loader::loadSpritesheet(const std::string &filename) {
  std::ifstream file(filename);
  std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  auto spritesheet = std::make_shared<SpriteSheet>();

  ENGLog("TEXT %s", text.c_str());

  return spritesheet;
}
