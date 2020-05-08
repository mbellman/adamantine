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

  auto stageStats = videoController->getScene()->getStage().getStats();

  sprintf_s(title, sizeof(title), "FPS: %d, Vertices: %d, Lights: %d, Shadowcasters: %d", stats.getFPS(), stageStats.totalVertices, stageStats.totalLights, stageStats.totalShadowCasters);

  SDL_SetWindowTitle(videoController->getWindow(), title);
}

void Window::open(const char* title, Region2d<int> region) {
  videoController->initWindow(title, region);
  videoController->onInit();
}

void Window::run() {
  int lastTick = SDL_GetTicks();

  while (videoController->isActive()) {
    float dt = (SDL_GetTicks() - lastTick) / 1000.0f;

    lastTick = SDL_GetTicks();

    stats.trackFrameStart();
    videoController->update(dt);
    videoController->onRender();
    stats.trackFrameEnd();
    handleStats();

    SDL_Delay(1);
  }
}

void Window::setVideoController(AbstractVideoController* videoController) {
  this->videoController = videoController;
}