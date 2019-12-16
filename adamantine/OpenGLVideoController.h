#pragma once

#include "AbstractVideoController.h"
#include "glut.h"

class OpenGLVideoController final : public AbstractVideoController {
public:
  SDL_Window* createWindow(const char* title, Region2d<int> region) override;
  void onDestroy() override;
  void onInit() override;
  void onRender() override;

private:
  SDL_GLContext glContext;
  GLint time;
};