#pragma once

#include <functional>

#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/FrameBuffer.h"
#include "subsystem/Math.h"

typedef std::function<FrameBuffer*(const Region2d<int>&, const ShaderProgram&)> FrameBufferFactory;
typedef std::function<void(const ShaderProgram&)> RenderHandler;

class ScreenShader {
public:
  ScreenShader(const char* shaderPath);
  ~ScreenShader();

  void createFrameBuffer(const Region2d<int>& screen);
  void onCreateFrameBuffer(FrameBufferFactory factory);
  void onRender(RenderHandler handler);
  void render();
  void startWriting();

private:
  ShaderProgram program;
  OpenGLPipeline* glScreenQuad = nullptr;
  FrameBuffer* frameBuffer = nullptr;
  FrameBufferFactory frameBufferFactory = nullptr;
  RenderHandler renderHandler = nullptr;
};