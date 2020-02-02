#include "opengl/OpenGLShadowCaster.h"
#include "opengl/FrameBuffer.h"

OpenGLShadowCaster::OpenGLShadowCaster(const Light* light) {
  this->light = light;

  frameBuffer = new FrameBuffer(1024, 1024);

  frameBuffer->addDepthTexture();
}

const Light* OpenGLShadowCaster::getLight() const {
  return light;
}

Matrix4 OpenGLShadowCaster::getLightMatrix() const {
  // Matrix4 projection = Matrix4::projection({ 0, 0, 1200, 720}, 45.0f, 1.0f, 1000.0f);
  Matrix4 projection = Matrix4::orthographic(200.0f, -200.0f, -200.0f, 200.0f, -1000.0f, 1000.0f);
  Matrix4 view = Matrix4::lookAt(Vec3f(-100.0f, -100.0f, 0.0f), light->direction * Vec3f(-1.0f, -1.0f, 1.0f));

  return projection * view;
}

Matrix4 OpenGLShadowCaster::getLightMatrixCascade(int cascadeIndex, const Camera& camera) const {
  const float* sizes = OpenGLShadowCaster::cascadeSizes[cascadeIndex];
  const float range = sizes[0];
  const float depth = sizes[1];

  Vec3f lightPosition = (camera.position + camera.getDirection().unit() * range);
  Matrix4 projection = Matrix4::orthographic(range, -range, -range, range, -depth, depth);
  Matrix4 view = Matrix4::lookAt(lightPosition * Vec3f(-1.0f, -1.0f, 1.0f), light->direction * Vec3f(-1.0f, -1.0f, 1.0f));

  return (projection * view).transpose();
}

void OpenGLShadowCaster::startCasting() {
  frameBuffer->startWriting();
}

void OpenGLShadowCaster::startReading() {
  frameBuffer->startReading();
}

const float OpenGLShadowCaster::cascadeSizes[3][2] = {
  { 100.0f, 1000.0f },
  { 300.0f, 3000.0f },
  { 1500.0f, 5000.0f }
};