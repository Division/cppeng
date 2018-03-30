//
// Created by Sidorenko Nikita on 1/17/18.
//

#ifndef TESTLIBRARY_ENGINE_H
#define TESTLIBRARY_ENGINE_H

#include <IGame.h>

class Window;
class Renderer;
class Input;
class Scene;

class Engine {
public:
  Engine();
  ~Engine();

  void setup(IGame *game);
  void quit();
  void printStatus();
  void update(double dt);
  void renderScene(Scene &scene);

  const Renderer *renderer() { return _renderer; }

  friend void mainLoop(void *arg);

private:
  void startEngineLoop();
#ifdef __EMSCRIPTEN__
  void startEmscriptenLoop();
#endif
  void init();

private:
  Window *_window;
  Renderer *_renderer;
  Input *_input;
  IGame *_game;

  bool _shouldQuit;
  double _currentTime = 0;
  double _startTime = 0;
};



#endif //TESTLIBRARY_ENGINE_H
