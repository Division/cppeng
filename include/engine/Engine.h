//
// Created by Sidorenko Nikita on 1/17/18.
//

#ifndef TESTLIBRARY_ENGINE_H
#define TESTLIBRARY_ENGINE_H

class Window;
class Renderer;
class Input;

class Engine {
public:
  Engine();
  ~Engine();

  void setupSDL();
  void quit();
  void printStatus();
  void update(double dt);

  friend void mainLoop(void *arg);

private:
  void startSDLLoop();
#ifdef __EMSCRIPTEN__
  void startEmscriptenLoop();
#endif
  void init();

private:
  Window *_window;
  Renderer *_renderer;
  Input *_input;

  bool _shouldQuit;
  double _currentTime = 0;
  double _startTime = 0;
};



#endif //TESTLIBRARY_ENGINE_H
