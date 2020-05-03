#include "opengl/OpenGLShadowCaster.h"
#include "opengl/FrameBuffer.h"

OpenGLShadowCaster::OpenGLShadowCaster(const Light* light) {
  this->light = light;
}

const Light* OpenGLShadowCaster::getLight() const {
  return light;
}

Matrix4 OpenGLShadowCaster::getCascadedLightMatrix(int cascadeIndex, const Camera& camera) const {
  const float* sizes = OpenGLShadowCaster::cascadeSizes[cascadeIndex];
  const float range = sizes[0];
  const float depth = sizes[1];

  Vec3f lightPosition = (camera.position + camera.getDirection().unit() * range);
  Matrix4 projection = Matrix4::orthographic(range, -range, -range, range, -depth, depth);
  Matrix4 view = Matrix4::lookAt(lightPosition.invert().gl(), light->direction.invert().gl(), Vec3f(0.0f, 1.0f, 0.0f));

  return (projection * view).transpose();
}

Matrix4 OpenGLShadowCaster::getLightMatrix(const Vec3f& direction, const Vec3f& top) const {
  Matrix4 projection = Matrix4::projection({ 0, 0, 1024, 1024 }, 90.0f, 1.0f, light->radius + 1000.0f);
  Matrix4 view = Matrix4::lookAt(light->position.invert().gl(), direction.invert().gl(), top);

  return (projection * view).transpose();
}

const float OpenGLShadowCaster::cascadeSizes[3][2] = {
  { 100.0f, 1000.0f },
  { 300.0f, 3000.0f },
  { 1000.0f, 5000.0f }
};