#pragma once

#include "SDL.h"
#include "Math.h"
#include "subsystem/AbstractVideoController.h"
#include "subsystem/InputSystem.h"
#include "Stats.h"

class Window {
public:
  Window();
  ~Window();

  void open(const char* title, Region2d<int> region);
  void run();
  void setVideoController(AbstractVideoController* videoController);

private:
  AbstractVideoController* videoController = 0;
  InputSystem inputSystem;
  Stats stats;

  void handleStats();
};