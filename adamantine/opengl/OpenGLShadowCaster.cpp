#include "opengl/OpenGLShadowCaster.h"
#include "opengl/FrameBuffer.h"

OpenGLShadowCaster::OpenGLShadowCaster(const Light* light) {
  this->light = light;
}

const Light* OpenGLShadowCaster::getLight() const {
  return light;
}

Matrix4 OpenGLShadowCaster::getLightMatrixCascade(int cascadeIndex, const Camera& camera) const {
  Matrix4 projection;
  Matrix4 view;

  if (light->type == Light::LightType::DIRECTIONAL) {
    const float* sizes = OpenGLShadowCaster::cascadeSizes[cascadeIndex];
    const float range = sizes[0];
    const float depth = sizes[1];

    Vec3f lightPosition = (camera.position + camera.getDirection().unit() * range);

    projection = Matrix4::orthographic(range, -range, -range, range, -depth, depth);
    view = Matrix4::lookAt(lightPosition * Vec3f(-1.0f, -1.0f, 1.0f), light->direction * Vec3f(-1.0f, -1.0f, 1.0f));
  } else {
    projection = Matrix4::projection({ 0, 0, 1200, 720 }, 80.0f, 1.0f, light->radius + 1000.0f);
    view = Matrix4::lookAt(light->position * Vec3f(-1.0f, -1.0f, 1.0f), light->direction * Vec3f(-1.0f, -1.0f, 1.0f));
  }

  return (projection * view).transpose();
}

const float OpenGLShadowCaster::cascadeSizes[3][2] = {
  { 100.0f, 1000.0f },
  { 300.0f, 3000.0f },
  { 1500.0f, 5000.0f }
};