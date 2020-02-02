#pragma once

#include "opengl/FrameBuffer.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "subsystem/Math.h"

class GBuffer {
public:
  GBuffer();
  ~GBuffer();

  void clearLightViewBuffer();
  void createFrameBuffer(int width, int height);
  ShaderProgram& getGeometryProgram();
  ShaderProgram& getLightingProgram();
  ShaderProgram& getLightViewProgram();
  ShaderProgram& getShadowCasterProgram();
  void renderScreenQuad();
  void startReading();
  void startWriting();
  void writeToAllBuffers();
  void writeToShadowCascade(int cascadeIndex);

private:
  ShaderProgram geometryProgram;
  ShaderProgram lightingProgram;
  ShaderProgram lightViewProgram;
  ShaderProgram shadowCasterProgram;
  FrameBuffer* frameBuffer = nullptr;
  OpenGLPipeline* glScreenQuad = nullptr;

  void createGeometryProgram();
  void createLightingProgram();
  void createLightViewProgram();
  void createShadowCasterProgram();
};