#include "AbstractVideoController.h"

AbstractVideoController::~AbstractVideoController() {
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
}

void AbstractVideoController::createWindow(const char* title, Region2d<int> region) {
  sdlWindow = getWindow(title, region);
}

void AbstractVideoController::destroy() {}