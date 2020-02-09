#include "opengl/GBuffer.h"
#include "opengl/ShaderLoader.h"

GBuffer::GBuffer() {
  createGeometryProgram();
  createLightingProgram();
  createLightViewProgram();
  createShadowCasterProgram();

  glScreenQuad = new OpenGLPipeline();

  lightingProgram.bindVertexInputs();
  shadowCasterProgram.bindVertexInputs();

  glScreenQuad->createScreenQuad();
}

GBuffer::~GBuffer() {

}

void GBuffer::clearLightViewBuffers() {
  frameBuffer->clearColorTexture(3);
  frameBuffer->clearColorTexture(4);
  frameBuffer->clearColorTexture(5);
}

void GBuffer::createFrameBuffer(int width, int height) {
  if (frameBuffer != nullptr) {
    delete frameBuffer;
  }

  frameBuffer = new FrameBuffer(width, height);

  frameBuffer->addColorTexture(GL_RGBA32F, GL_RGBA);   // (0) Color/lighting flag
  frameBuffer->addColorTexture(GL_RGBA32F, GL_RGBA);   // (1) Normal/depth
  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB);     // (2) Position
  frameBuffer->addColorTexture(GL_R32F, GL_RED);       // (3) Shadowcaster light view buffer, cascade 0
  frameBuffer->addColorTexture(GL_R32F, GL_RED);       // (4) Shadowcaster light view buffer, cascade 1
  frameBuffer->addColorTexture(GL_R32F, GL_RED);       // (5) Shadowcaster light view buffer, cascade 2
  frameBuffer->addDepthBuffer();
  frameBuffer->bindColorTextures();
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
  lightingProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/light.fragment.glsl"));
  lightingProgram.link();
  lightingProgram.use();

  VertexShaderInput inputs[] = {
    { "vertexPosition", 2, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  lightingProgram.setVertexInputs<float>(2, inputs);
}

void GBuffer::createLightViewProgram() {
  lightViewProgram.create();
  lightViewProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/lightview.vertex.glsl"));
  lightViewProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/lightview.fragment.glsl"));
  lightViewProgram.link();
  lightViewProgram.use();

  VertexShaderInput vertexInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  lightViewProgram.setVertexInputs<float>(5, vertexInputs);
}

void GBuffer::createShadowCasterProgram() {
  shadowCasterProgram.create();
  shadowCasterProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/quad.vertex.glsl"));
  shadowCasterProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/shadowcaster.fragment.glsl"));
  shadowCasterProgram.link();
  shadowCasterProgram.use();

  VertexShaderInput inputs[] = {
    { "vertexPosition", 2, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  shadowCasterProgram.setVertexInputs<float>(2, inputs);
}

ShaderProgram& GBuffer::getGeometryProgram() {
  return geometryProgram;
}

ShaderProgram& GBuffer::getLightingProgram() {
  return lightingProgram;
}

ShaderProgram& GBuffer::getLightViewProgram() {
  return lightViewProgram;
}

ShaderProgram& GBuffer::getShadowCasterProgram() {
  return shadowCasterProgram;
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

void GBuffer::useFirstShadowCascade() {
  GLenum targets[] = {
    GL_COLOR_ATTACHMENT0 + 4,
    GL_COLOR_ATTACHMENT0 + 5
  };

  frameBuffer->transferColorTexture(GL_COLOR_ATTACHMENT0 + 3, targets, 2);
}

void GBuffer::writeToAllBuffers() {
  frameBuffer->bindColorTextures();
}

void GBuffer::writeToShadowCascade(int cascadeIndex) {
  frameBuffer->bindColorTexture(GL_COLOR_ATTACHMENT0 + 3 + cascadeIndex);
}