//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_IGAME_H
#define CPPWRAPPER_IGAME_H

class Engine;

class IGame {
public:
  virtual void init(Engine *engine) = 0;
  virtual void update(float dt) = 0;
};

#endif //CPPWRAPPER_IGAME_H
