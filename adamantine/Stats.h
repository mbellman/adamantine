#pragma once

#include "Math.h"
#include "SDL.h"

typedef int SDL_Tick;

class Stats {
public:
  int getFPS();
  void trackFrameStart();
  void trackFrameEnd();

private:
  Range<SDL_Tick> frame = { 0, 0 };
};