#pragma once

#include <map>

#include "opengl/AbstractBuffer.h"
#include "opengl/FrameBuffer.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "subsystem/Math.h"

class GBuffer : public AbstractBuffer {
public:
  enum Shader {
    GEOMETRY,
    ILLUMINATION,
    ALBEDO
  };

  GBuffer();
  ~GBuffer();

  void createFrameBuffer(unsigned int width, unsigned int height) override;
  ShaderProgram& getShaderProgram(GBuffer::Shader shader);
  void renderScreenQuad();
  void writeToAllBuffers();

protected:
  void createShaderPrograms() override;

private:
  ShaderProgram geometryProgram;
  ShaderProgram illuminationProgram;
  ShaderProgram albedoProgram;
};