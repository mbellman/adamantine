#pragma once

#include "AbstractVideoController.h"

class OpenGLVideoController final : public AbstractVideoController {
public:
  void destroy() override;
  void init() override;
  void render() override;

private:
  SDL_GLContext glContext;

  SDL_Window* getWindow(const char* title, Region2d<int> region) override;
};