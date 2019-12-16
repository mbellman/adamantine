#include <cstdio>
#include <cmath>

#include "Window.h"
#include "SDL.h"
#include "Stats.h"

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
  SDL_Init(SDL_INIT_EVERYTHING);

  videoController->initWindow(title, region);
  videoController->onInit();
}

void Window::poll() {
  bool isClosed = false;
  SDL_Event event;

  while (!isClosed) {
    stats.trackFrameStart();

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          isClosed = true;
          break;
      }
    }

    videoController->onRender();
    stats.trackFrameEnd();
    handleStats();

    SDL_Delay(1);
  }
}

void Window::setVideoController(AbstractVideoController* videoController) {
  this->videoController = videoController;
}