#pragma once

#include "SDL.h"
#include "Geometry.h"

class AbstractVideoController {
public:
  ~AbstractVideoController();

  virtual void createWindow(const char* title, Region2d<int> region) final;
  virtual void destroy();
  virtual void init() = 0;
  virtual void render() = 0;

protected:
  SDL_Window* sdlWindow = 0;

  virtual SDL_Window* getWindow(const char* title, Region2d<int> region) = 0;
};