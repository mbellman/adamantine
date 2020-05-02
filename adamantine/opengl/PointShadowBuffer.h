#pragma once

#include "opengl/AbstractBuffer.h"
#include "opengl/ShaderProgram.h"
#include "opengl/OpenGLPipeline.h"

class PointShadowBuffer : public AbstractBuffer {
public:
  PointShadowBuffer();

  void createFrameBuffer(unsigned int width, unsigned int height) override;
  ShaderProgram& getPointLightViewProgram();
  ShaderProgram& getPointShadowProgram();

protected:
  void createShaderPrograms() override;

private:
  ShaderProgram pointLightViewProgram;
  ShaderProgram pointShadowProgram;
};