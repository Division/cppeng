//
// Created by Sidorenko Nikita on 11/17/18.
//
#include "catch.hpp"
#include "objects/Camera.h"
#include "render/shader/Shader.h"
#include <unordered_map>
#include <memory>

TEST_CASE( "Shader caps" ) {

  std::unordered_map<unsigned int, bool> capsCache;

  ShaderCapsSet caps;

  for (int i = 0; i < (int)ShaderCaps::Count; i++) {
    auto cap = (ShaderCaps)i;
    caps.addCap(cap);
    REQUIRE(capsCache.count(caps.getBitmask()) == 0);
    capsCache[caps.getBitmask()] = true;
  }

}
