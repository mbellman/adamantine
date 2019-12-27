#include <cstdio>
#include <cmath>

#include "Window.h"
#include "SDL.h"
#include "subsystem/InputSystem.h"
#include "Stats.h"
#include "subsystem/RNG.h"

Window::Window() {
  SDL_Init(SDL_INIT_EVERYTHING);
}

Window::~Window() {
  videoController->onDestroy();

  delete videoController;
}

void Window::handleStats() {
  char title[100];
  sprintf_s(title, sizeof(title), "FPS: %d", stats.getFPS());

  SDL_SetWindowTitle(videoController->getWindow(), title);
}

void Window::open(const char* title, Region2d<int> region) {
  videoController->initWindow(title, region);
  videoController->onInit();
}

void Window::run() {
  while (!videoController->isActive()) {
    stats.trackFrameStart();
    videoController->update();
    videoController->onRender();
    stats.trackFrameEnd();
    handleStats();

    SDL_Delay(1);
  }
}

void Window::setVideoController(AbstractVideoController* videoController) {
  this->videoController = videoController;
}