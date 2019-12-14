#include "Window.h";

Window::Window(const char* title, int x, int y, int width, int height) {
  SDL_Init(SDL_INIT_VIDEO);

  sdlWindow = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL);

  poll();
}

Window::~Window() {
  SDL_DestroyWindow(sdlWindow);
  SDL_Quit();
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

    SDL_Delay(1);
  }
}
