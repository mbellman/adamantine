#pragma once

#include "SDL.h";

class Window {
public:
  Window(const char* title, int x, int y, int width, int height);
  ~Window();
private:
  SDL_Window* sdlWindow = 0;

  void poll();
};
