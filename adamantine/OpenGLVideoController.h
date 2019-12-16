#pragma once

#include "AbstractVideoController.h"
#include "OpenGLPipeline.h"
#include "ShaderProgram.h"
#include "glut.h"

class OpenGLVideoController final : public AbstractVideoController {
public:
  SDL_Window* createWindow(const char* title, Region2d<int> region) override;
  void onDestroy() override;
  void onInit() override;
  void onRender() override;
  void onScreenSizeChange(int width, int height) override;

private:
  SDL_GLContext glContext;
  OpenGLPipeline pipeline;
  ShaderProgram shaderProgram;
  GLint time;
};