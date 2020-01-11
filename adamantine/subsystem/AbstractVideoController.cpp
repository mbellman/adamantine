#include "subsystem/AbstractVideoController.h"

AbstractVideoController::~AbstractVideoController() {
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
}

SDL_Window* AbstractVideoController::getWindow() {
  return sdlWindow;
}

void AbstractVideoController::initWindow(const char* title, Region2d<int> region) {
  sdlWindow = createWindow(title, region);

  screenSize.width = region.width;
  screenSize.height = region.height;
}

bool AbstractVideoController::isActive() const {
  return scene->isActive();
}

void AbstractVideoController::onDestroy() {}

void AbstractVideoController::onScreenSizeChange(int width, int height) {}

void AbstractVideoController::update(float dt) {
  scene->pollInput();
  scene->onUpdate(dt);
  scene->updateRunningTime(dt);

  for (auto* object : scene->getStage().getObjects()) {
    if (object->onUpdate) {
      object->onUpdate(dt);
    }
  }
}

void AbstractVideoController::setScene(AbstractScene* scene) {
  this->scene = scene;
}

void AbstractVideoController::toggleFullScreen() {
  int flags = SDL_GetWindowFlags(sdlWindow);
  bool isFullScreen = flags & SDL_WINDOW_FULLSCREEN;

  SDL_DisplayMode display;

  SDL_GetDesktopDisplayMode(0, &display);

  int fullScreenFlags = isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN;
  int updatedWidth = isFullScreen ? 640 : display.w;
  int updatedHeight = isFullScreen ? 480 : display.h;

  SDL_SetWindowFullscreen(sdlWindow, isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN);
  SDL_SetWindowSize(sdlWindow, updatedWidth, updatedHeight);

  onScreenSizeChange(updatedWidth, updatedHeight);
}
