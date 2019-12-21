#pragma once

#include <vector>

#include "AbstractVideoController.h"
#include "ShaderProgram.h"
#include "VertexPipeline.h"
#include "OpenGLScene.h"
#include "Geometry.h"
#include "Entities.h"
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
  ShaderProgram shaderProgram;
  OpenGLScene scene;

  Matrix4 createProjectionMatrix(float fov, float aspectRatio, float near, float far);
  Matrix4 createViewMatrix();
};