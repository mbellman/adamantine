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
#include "opengl/SBuffer.h"
#include "opengl/PointShadowBuffer.h"
#include "subsystem/Geometry.h"
#include "subsystem/Entities.h"
#include "subsystem/HeapList.h"
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
  SBuffer* sBuffer = nullptr;
  PointShadowBuffer* pointShadowBuffer = nullptr;
  HeapList<ScreenShader> screenShaders;
  HeapList<OpenGLObject> glObjects;
  HeapList<OpenGLShadowCaster> glShadowCasters;

  void checkErrors();
  void createScreenShaders();
  Matrix4 createViewMatrix();
  void onEntityAdded(Entity* entity);
  void onEntityRemoved(Entity* entity);
  void renderDirectionalShadowCaster(OpenGLShadowCaster* glShadowCaster);
  void renderEmissiveSurfaces();
  void renderGeometry();
  void renderIlluminatedSurfaces();
  void renderPointShadowCaster(OpenGLShadowCaster* glShadowCaster);
  void renderScreenShaders();
  void renderShadowCasters();
  void renderSpotShadowCaster(OpenGLShadowCaster* glShadowCaster);
};