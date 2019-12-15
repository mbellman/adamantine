#pragma once

#include "SDL.h"
#include "Geometry.h"
#include "AbstractVideoController.h"

class Window {
public:
  ~Window();

  void open(const char* title, Region2d<int> region);
  void poll();
  void setVideoController(AbstractVideoController* videoController);

private:
  SDL_Window* sdlWindow = 0;
  AbstractVideoController* videoController = 0;
};
