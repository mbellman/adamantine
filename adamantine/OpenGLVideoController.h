#pragma once

#include <vector>

#include "AbstractVideoController.h"
#include "ShaderProgram.h"
#include "VertexPipeline.h"
#include "Scene.h"
#include "Geometry.h"
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
  std::vector<VertexPipeline*> pipelines;
  Scene scene;
  GLint cameraMatrix;
  GLint time;

  void createVertexPipelineFromObject(Object* object);
};