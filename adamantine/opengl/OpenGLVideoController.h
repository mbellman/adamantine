#pragma once

#include <vector>

#include "subsystem/AbstractVideoController.h"
#include "opengl/ShaderProgram.h"
#include "opengl/VertexPipeline.h"
#include "opengl/OpenGLScene.h"
#include "subsystem/Geometry.h"
#include "subsystem/Entities.h"
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

  Matrix4 createProjectionMatrix(float fov, float aspectRatio, float near, float far);
  Matrix4 createViewMatrix();
};