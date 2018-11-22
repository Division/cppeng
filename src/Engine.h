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
class ShaderGenerator;
class Shader;
class ShaderCapsSet;
class SceneRenderer;
class Texture;

typedef std::shared_ptr<Shader> ShaderPtr;

class Engine {
public:
  Engine();
  ~Engine();

  void setup(std::weak_ptr<IGame> game);
  void quit();
  void printStatus();
  void update(double dt);
  void renderScene(std::shared_ptr<Scene> scene);

  std::shared_ptr<ShaderGenerator> generator() { return _generator; }
  ShaderPtr getShaderWithCaps (std::shared_ptr<ShaderCapsSet> caps) const;

  std::shared_ptr<DebugDraw> debugDraw() const;

  const std::shared_ptr<Input> input() const { return _input; }
  double time () { return _currentTime; }

  void projectorTexture(const std::shared_ptr<Texture> texture);

  const std::shared_ptr<Window>window() { return _window; }

  friend void mainLoop(void *arg);

private:
  void startEngineLoop();
#ifdef __EMSCRIPTEN__
  void startEmscriptenLoop();
#endif
  void init();

private:
  std::shared_ptr<SceneRenderer> _sceneRenderer;
  std::shared_ptr<Window> _window;
  std::shared_ptr<ShaderGenerator> _generator;
  std::shared_ptr<Input> _input;
  std::weak_ptr<IGame> _game;

  bool _shouldQuit = false;
  double _currentTime = 0;
  double _startTime = 0;
};



#endif //TESTLIBRARY_ENGINE_H
