#include "opengl/ScreenShader.h"

ScreenShader::ScreenShader(const char* shaderPath) {
  program.create();
  program.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/quad.vertex.glsl"));
  program.attachShader(ShaderLoader::loadFragmentShader(shaderPath));
  program.link();
  program.use();

  VertexShaderInput inputs[] = {
    { "vertexPosition", 2, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  program.setVertexInputs<float>(2, inputs);

  glScreenQuad = new OpenGLPipeline();

  program.bindVertexInputs();

  glScreenQuad->createScreenQuad();
}

ScreenShader::~ScreenShader() {
  delete glScreenQuad;
  delete frameBuffer;
}

void ScreenShader::createFrameBuffer(const Region2d<int>& screen) {
  if (frameBuffer != nullptr) {
    delete frameBuffer;
  }

  program.use();

  frameBuffer = frameBufferFactory(program, screen);
}

FrameBuffer* ScreenShader::getFrameBuffer() const {
  return frameBuffer;
}

void ScreenShader::onCreateFrameBuffer(FrameBufferFactory factory) {
  frameBufferFactory = factory;
}

void ScreenShader::onRender(RenderHandler handler) {
  renderHandler = handler;
}

void ScreenShader::render() {
  frameBuffer->startReading();
  program.use();

  renderHandler(program, glScreenQuad);
}

void ScreenShader::startWriting() {
  frameBuffer->startWriting();
}