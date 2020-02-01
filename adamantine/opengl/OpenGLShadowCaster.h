#pragma once

#include <vector>

#include "subsystem/Entities.h"
#include "subsystem/Math.h"
#include "opengl/FrameBuffer.h"
#include "opengl/OpenGLObject.h"

class OpenGLShadowCaster {
public:
  OpenGLShadowCaster(const Light* light);

  const Light* getLight() const;
  Matrix4 getLightMatrix() const;
  void startCasting();
  void startReading();

private:
  const Light* light = nullptr;
  FrameBuffer* frameBuffer = nullptr;
};