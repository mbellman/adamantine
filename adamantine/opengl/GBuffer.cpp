#include "opengl/GBuffer.h"
#include "opengl/ShaderLoader.h"

GBuffer::GBuffer() {
  createShaderPrograms();

  glScreenQuad = new OpenGLPipeline();

  getShaderProgram(Shader::ILLUMINATION).bindVertexInputs();
  getShaderProgram(Shader::SHADOW_CASTER).bindVertexInputs();
  getShaderProgram(Shader::ALBEDO).bindVertexInputs();

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

  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB);     // (0) Color
  frameBuffer->addColorTexture(GL_RGBA32F, GL_RGBA);   // (1) Normal/depth
  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB);     // (2) Position
  frameBuffer->addColorTexture(GL_R32F, GL_RED);       // (3) Shadowcaster light view buffer, cascade 0
  frameBuffer->addColorTexture(GL_R32F, GL_RED);       // (4) Shadowcaster light view buffer, cascade 1
  frameBuffer->addColorTexture(GL_R32F, GL_RED);       // (5) Shadowcaster light view buffer, cascade 2
  frameBuffer->addDepthStencilBuffer();
  frameBuffer->bindColorTextures();
}

void GBuffer::createShaderPrograms() {
  VertexShaderInput geometryInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  VertexShaderInput quadInputs[] = {
    { "vertexPosition", 2, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  // Geometry program
  geometryProgram.create();
  geometryProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/geometry.vertex.glsl"));
  geometryProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/geometry.fragment.glsl"));
  geometryProgram.link();
  geometryProgram.use();
  geometryProgram.setVertexInputs<float>(5, geometryInputs);

  // Lighting program
  illuminationProgram.create();
  illuminationProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/quad.vertex.glsl"));
  illuminationProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/illumination.fragment.glsl"));
  illuminationProgram.link();
  illuminationProgram.use();
  illuminationProgram.setVertexInputs<float>(2, quadInputs);

  // Light view program
  lightViewProgram.create();
  lightViewProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/lightview.vertex.glsl"));
  lightViewProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/lightview.fragment.glsl"));
  lightViewProgram.link();
  lightViewProgram.use();
  lightViewProgram.setVertexInputs<float>(5, geometryInputs);

  // Shadow caster program
  shadowCasterProgram.create();
  shadowCasterProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/quad.vertex.glsl"));
  shadowCasterProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/shadowcaster.fragment.glsl"));
  shadowCasterProgram.link();
  shadowCasterProgram.use();
  shadowCasterProgram.setVertexInputs<float>(2, quadInputs);

  // Albedo program
  albedoProgram.create();
  albedoProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/quad.vertex.glsl"));
  albedoProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/albedo.fragment.glsl"));
  albedoProgram.link();
  albedoProgram.use();
  albedoProgram.setVertexInputs<float>(2, quadInputs);
}

FrameBuffer* GBuffer::getFrameBuffer() {
  return frameBuffer;
}

ShaderProgram& GBuffer::getShaderProgram(GBuffer::Shader shader) {
  switch (shader) {
    case GBuffer::Shader::GEOMETRY:
      return geometryProgram;
    case GBuffer::Shader::ILLUMINATION:
      return illuminationProgram;
    case GBuffer::Shader::LIGHT_VIEW:
      return lightViewProgram;
    case GBuffer::Shader::SHADOW_CASTER:
      return shadowCasterProgram;
    case GBuffer::Shader::ALBEDO:
      return albedoProgram;
    default:
      return geometryProgram;
  }
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