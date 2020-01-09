#pragma once

#include <vector>
#include <map>

#include "subsystem/AbstractVideoController.h"
#include "opengl/ShaderProgram.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/GBuffer.h"
#include "subsystem/Geometry.h"
#include "subsystem/Entities.h"
#include "glut.h"

class OpenGLVideoController final : public AbstractVideoController {
public:
  OpenGLVideoController();
  ~OpenGLVideoController();

  SDL_Window* createWindow(const char* title, Region2d<int> region) override;
  void onDestroy() override;
  void onInit() override;
  void onRender() override;
  void onScreenSizeChange(int width, int height) override;

private:
  SDL_GLContext glContext;
  ShaderProgram geometry;
  ShaderProgram lighting;
  GBuffer gBuffer;
  OpenGLPipeline* lightingPipeline = nullptr;
  std::map<int, OpenGLPipeline*> pipelines;

  void createLightingProgram();
  void createGeometryProgram();
  OpenGLPipeline* createOpenGLPipeline(const Object* object);
  Matrix4 createProjectionMatrix(float fov, float aspectRatio, float near, float far);
  Matrix4 createViewMatrix();
  void onEntityAdded(Entity* entity);
  void onEntityRemoved(Entity* entity);
};