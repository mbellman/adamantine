#pragma once

#include "opengl/FrameBuffer.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "subsystem/Math.h"

class GBuffer {
public:
  GBuffer();
  ~GBuffer();

  void createFrameBuffer(int width, int height);
  ShaderProgram& getDepthProgram();
  ShaderProgram& getGeometryProgram();
  ShaderProgram& getLightingProgram();
  void renderScreenQuad();
  void startReading();
  void startWriting();

private:
  ShaderProgram geometryProgram;
  ShaderProgram depthProgram;
  ShaderProgram lightingProgram;
  FrameBuffer* frameBuffer = nullptr;
  OpenGLPipeline* glScreenQuad = nullptr;

  void createDepthProgram();
  void createGeometryProgram();
  void createLightingProgram();
};