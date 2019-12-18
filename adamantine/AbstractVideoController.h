#pragma once

#include "SDL.h"
#include "Math.h"

class AbstractVideoController {
public:
  virtual ~AbstractVideoController();

  virtual SDL_Window* createWindow(const char* title, Region2d<int> region) = 0;
  virtual SDL_Window* getWindow() final;
  virtual void initWindow(const char* title, Region2d<int> region) final;
  virtual void onDestroy();
  virtual void onInit() = 0;
  virtual void onRender() = 0;
  virtual void onScreenSizeChange(int width, int height);
  virtual void toggleFullScreen() final;

protected:
  SDL_Window* sdlWindow = 0;
};