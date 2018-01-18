//
// Created by Sidorenko Nikita on 1/17/18.
//

#ifndef TESTLIBRARY_ENGINE_H
#define TESTLIBRARY_ENGINE_H

#import "render/Mesh.h"


class Engine {
public:
  Engine();
  void setupSDL();
  void quit();
  void printStatus();

private:
//  void mainLoop();
  void startSDLLoop();
  void startEmscriptenLoop();

private:
  bool _shouldQuit;
};



#endif //TESTLIBRARY_ENGINE_H
