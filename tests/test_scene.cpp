//
// Created by Sidorenko Nikita on 3/29/18.
//
#include "catch.hpp"
#include "../src/scene/Scene.h"
#include "../src/EngMath.h"
#include "../src/system/Logging.h"

TEST_CASE( "Game object transform" ) {
  auto scene = new Scene();
  scene->setAsDefault();

  auto obj1 = CreateGameObject<GameObject>();
  auto obj2 = CreateGameObject<GameObject>();

  obj2->transform()->setParent(obj1->transform());
  obj2->transform()->setPosition(vec3(10, 0, 0));
  obj1->transform()->rotate(vec3(1, 0, 0), RAD(45));

  scene->update(0.01);

  REQUIRE(scene->gameObjects().size() == 2);
  REQUIRE(obj2->transform()->parent() == obj1->transform());

  SECTION( "Rotate x" ) {
    vec4 worldPos = (obj2->transform()->worldMatrix() * vec4(obj2->transform()->position(), 1));
    REQUIRE(worldPos.x == 10);

    ENGLog("m1: %s", to_string(obj2->transform()->worldMatrix()).c_str());

    obj2->transform()->setPosition(vec3(0, 10, 0));
    scene->update(0.01);

    ENGLog("m2: %s", to_string(obj2->transform()->worldMatrix()).c_str());
    worldPos = obj2->transform()->worldMatrix() * vec4(obj2->transform()->position(), 1);

    REQUIRE(worldPos.y == 0);
  }
}
