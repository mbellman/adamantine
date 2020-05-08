#pragma once

#include "SDL.h"
#include "subsystem/Math.h"
#include "subsystem/AbstractScene.h"

class AbstractVideoController {
public:
  virtual ~AbstractVideoController();

  virtual SDL_Window* createWindow(const char* title, Region2d<int> region) = 0;
  virtual const AbstractScene* getScene() const final;
  virtual SDL_Window* getWindow() final;
  virtual void initWindow(const char* title, Region2d<int> region) final;
  virtual bool isActive() const final;
  virtual void onDestroy();
  virtual void onInit() = 0;
  virtual void onRender() = 0;
  virtual void onScreenSizeChange(int width, int height);
  virtual void setScene(AbstractScene* scene);
  virtual void toggleFullScreen() final;
  virtual void update(float dt) final;

protected:
  bool didCloseWindow = false;
  SDL_Window* sdlWindow = nullptr;
  AbstractScene* scene = nullptr;
  Region2d<int> screenSize;

private:
  void pollEvents();
};