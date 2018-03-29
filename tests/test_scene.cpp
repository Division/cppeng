//
// Created by Sidorenko Nikita on 3/29/18.
//
#include "catch.hpp"
#include "../src/scene/Scene.h"

TEST_CASE( "Add game object" ) {
  auto scene = new Scene();
  auto obj = new GameObject();

  REQUIRE( scene->gameObjects().size() == 1 );
}
