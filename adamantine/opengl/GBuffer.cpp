#include "opengl/GBuffer.h"
#include "opengl/ShaderLoader.h"

GBuffer::GBuffer() {
  createGeometryProgram();
  createDepthProgram();
  createLightingProgram();
}

GBuffer::~GBuffer() {

}

void GBuffer::createDepthProgram() {
  depthProgram.create();
  depthProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/depth.vertex.glsl"));
  depthProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/depth.fragment.glsl"));
  depthProgram.link();
  depthProgram.use();

  VertexShaderInput vertexInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  depthProgram.setVertexInputs<float>(5, vertexInputs);
}

void GBuffer::createFrameBuffer(int width, int height) {
  if (frameBuffer != nullptr) {
    delete frameBuffer;
  }

  frameBuffer = new FrameBuffer(width, height);

  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB);     // Color
  frameBuffer->addColorTexture(GL_RGBA32F, GL_RGBA);   // Normal/depth
  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB);     // Position
  frameBuffer->addColorTexture(GL_RED, GL_RED);        // Shadowcaster light depth buffer
  frameBuffer->addDepthBuffer();
  frameBuffer->initializeColorTextures();
}

void GBuffer::createGeometryProgram() {
  geometryProgram.create();
  geometryProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/geometry.vertex.glsl"));
  geometryProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/geometry.fragment.glsl"));
  geometryProgram.link();
  geometryProgram.use();

  VertexShaderInput vertexInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  geometryProgram.setVertexInputs<float>(5, vertexInputs);
}

void GBuffer::createLightingProgram() {
  lightingProgram.create();
  lightingProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/quad.vertex.glsl"));
  lightingProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/lighting.glsl"));
  lightingProgram.link();
  lightingProgram.use();

  VertexShaderInput inputs[] = {
    { "vertexPosition", 2, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  lightingProgram.setVertexInputs<float>(2, inputs);

  glScreenQuad = new OpenGLPipeline();

  lightingProgram.bindVertexInputs();
  glScreenQuad->createScreenQuad();
}

ShaderProgram& GBuffer::getDepthProgram() {
  return depthProgram;
}

ShaderProgram& GBuffer::getGeometryProgram() {
  return geometryProgram;
}

ShaderProgram& GBuffer::getLightingProgram() {
  return lightingProgram;
}

void GBuffer::renderScreenQuad() {
  glScreenQuad->render();
}

void GBuffer::startReading() {
  frameBuffer->startReading();
}

void GBuffer::startWriting() {
  frameBuffer->startWriting();
}
