#include "AbstractVideoController.h"

AbstractVideoController::~AbstractVideoController() {
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
}

SDL_Window* AbstractVideoController::getWindow() {
  return sdlWindow;
}

void AbstractVideoController::initWindow(const char* title, Region2d<int> region) {
  sdlWindow = createWindow(title, region);
}

void AbstractVideoController::onDestroy() {}