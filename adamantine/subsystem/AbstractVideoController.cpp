#include "SDL.h"
#include "subsystem/AbstractVideoController.h"

AbstractVideoController::~AbstractVideoController() {
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
}

const AbstractScene* AbstractVideoController::getScene() const {
  return scene;
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
  return !didCloseWindow;
}

void AbstractVideoController::onDestroy() {}

void AbstractVideoController::onScreenSizeChange(int width, int height) {}

void AbstractVideoController::pollEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        didCloseWindow = true;
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          onScreenSizeChange(event.window.data1, event.window.data2);
        }

        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_f) {
          toggleFullScreen();
        }

        break;
      default:
        break;
    }

    scene->getInputSystem().handleEvent(event);
  }
}

void AbstractVideoController::update(float dt) {
  pollEvents();

  scene->update(dt);
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
  int updatedWidth = isFullScreen ? 1200 : display.w;
  int updatedHeight = isFullScreen ? 720 : display.h;

  SDL_SetWindowFullscreen(sdlWindow, isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN);
  SDL_SetWindowSize(sdlWindow, updatedWidth, updatedHeight);

  onScreenSizeChange(updatedWidth, updatedHeight);
}
