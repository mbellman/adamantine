#include "opengl/OpenGLShadowCaster.h"
#include "opengl/FrameBuffer.h"

OpenGLShadowCaster::OpenGLShadowCaster(const Light* light) {
  this->light = light;

  frameBuffer = new FrameBuffer(1024, 1024);

  frameBuffer->addDepthTexture();
}

Matrix4 OpenGLShadowCaster::getLightMatrix() const {
  // Matrix4 projection = Matrix4::projection({ 0, 0, 1200, 720}, 45.0f, 1.0f, 1000.0f);
  Matrix4 projection = Matrix4::orthographic(200.0f, -200.0f, -200.0f, 200.0f, -1000.0f, 1000.0f);
  Matrix4 view = Matrix4::lookAt(Vec3f(-100.0f, -100.0f, 0.0f), light->direction * Vec3f(-1.0f, -1.0f, 1.0f));

  return projection * view;
}

void OpenGLShadowCaster::startCasting() {
  frameBuffer->startWriting();
}

void OpenGLShadowCaster::startReading() {
  frameBuffer->startReading();
}