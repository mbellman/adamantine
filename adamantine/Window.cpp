#include "Window.h"
#include "SDL.h"

Window::~Window() {
  videoController->destroy();

  delete videoController;
}

void Window::open(const char* title, Region2d<int> region) {
  SDL_Init(SDL_INIT_EVERYTHING);

  videoController->createWindow(title, region);
  videoController->init();
}

void Window::poll() {
  bool isClosed = false;
  SDL_Event event;

  while (!isClosed) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          isClosed = true;
          break;
      }
    }

    videoController->render();
    SDL_Delay(1);
  }
}

void Window::setVideoController(AbstractVideoController* videoController) {
  this->videoController = videoController;
}