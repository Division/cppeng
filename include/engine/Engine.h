//
// Created by Sidorenko Nikita on 1/17/18.
//

#ifndef TESTLIBRARY_ENGINE_H
#define TESTLIBRARY_ENGINE_H

class Renderer;

class Engine {
public:
  Engine();
  void setupSDL();
  void quit();
  void printStatus();
  void update(double dt);
  void checkGLError();

  friend void mainLoop(void *arg);

private:
  void startSDLLoop();
#ifdef __EMSCRIPTEN__
  void startEmscriptenLoop();
#endif
  void init();

private:
  Renderer *_renderer;

  bool _shouldQuit;
  double _currentTime = 0;
  double _startTime = 0;
};



#endif //TESTLIBRARY_ENGINE_H
