#pragma once

#include <map>

#include "opengl/FrameBuffer.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "subsystem/Math.h"

class GBuffer {
public:
  enum Shader {
    GEOMETRY,
    ILLUMINATION,
    LIGHT_VIEW,
    SHADOW_CASTER,
    ALBEDO
  };

  GBuffer();
  ~GBuffer();

  void clearLightViewBuffers();
  void createFrameBuffer(int width, int height);
  FrameBuffer* getFrameBuffer();
  ShaderProgram& getShaderProgram(GBuffer::Shader shader);
  void renderScreenQuad();
  void startReading();
  void startWriting();
  void useFirstShadowCascade();
  void writeToAllBuffers();
  void writeToShadowCascade(int cascadeIndex);

private:
  ShaderProgram geometryProgram;
  ShaderProgram illuminationProgram;
  ShaderProgram lightViewProgram;
  ShaderProgram shadowCasterProgram;
  ShaderProgram albedoProgram;
  FrameBuffer* frameBuffer = nullptr;
  OpenGLPipeline* glScreenQuad = nullptr;

  void createShaderPrograms();
};