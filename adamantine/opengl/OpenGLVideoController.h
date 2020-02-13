#pragma once

#include <vector>
#include <map>

#include "subsystem/AbstractVideoController.h"
#include "opengl/ShaderProgram.h"
#include "opengl/OpenGLObject.h"
#include "opengl/OpenGLShadowCaster.h"
#include "opengl/FrameBuffer.h"
#include "opengl/ScreenShader.h"
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
  GBuffer* gBuffer = nullptr;
  std::vector<ScreenShader*> screenShaders;
  std::vector<OpenGLObject*> glObjects;
  std::vector<OpenGLShadowCaster*> glShadowCasters;

  OpenGLObject* createOpenGLObject(Object* object);
  void createScreenShaders();
  Matrix4 createViewMatrix();
  void onEntityAdded(Entity* entity);
  void onEntityRemoved(Entity* entity);
  void renderEmissiveSurfaces();
  void renderGeometry();
  void renderIlluminatedSurfaces();
  void renderScreenShaders();
  void renderShadowCasters();
};