#pragma once

#include "SDL.h"
#include "Math.h"
#include "AbstractVideoController.h"
#include "Stats.h"

class Window {
public:
  ~Window();

  void open(const char* title, Region2d<int> region);
  void poll();
  void setVideoController(AbstractVideoController* videoController);

private:
  AbstractVideoController* videoController = 0;
  Stats stats;

  void handleStats();
};