//
// Created by Sidorenko Nikita on 1/17/18.
//

#ifndef TESTLIBRARY_ENGINE_H
#define TESTLIBRARY_ENGINE_H

#include <IGame.h>
#include <memory>

class Window;
class Renderer;
class Input;
class Scene;
class DebugDraw;

class Engine {
public:
  Engine();
  ~Engine();

  void setup(IGame *game);
  void quit();
  void printStatus();
  void update(double dt);
  void renderScene(Scene &scene);
  std::shared_ptr<DebugDraw> debugDraw() const;

  const Input *const input() const { return _input; }

  const Renderer *renderer() { return _renderer; }
  const Window *window() { return _window; }

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

  float _lastDt;
  bool _shouldQuit;
  double _currentTime = 0;
  double _startTime = 0;
};



#endif //TESTLIBRARY_ENGINE_H
