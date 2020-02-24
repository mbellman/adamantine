#include "opengl/SBuffer.h"
#include "opengl/ShaderLoader.h"

SBuffer::SBuffer() {
  createShaderPrograms();
}

void SBuffer::createFrameBuffer(unsigned int width, unsigned int height) {
  if (frameBuffer != nullptr) {
    delete frameBuffer;
  }

  frameBuffer = new FrameBuffer(width, height);

  frameBuffer->addColorTexture(GL_R32F, GL_RED, GL_CLAMP_TO_BORDER, GL_TEXTURE3);   // Shadowcaster light view buffer, cascade 0
  frameBuffer->addColorTexture(GL_R32F, GL_RED, GL_CLAMP_TO_BORDER, GL_TEXTURE4);   // Shadowcaster light view buffer, cascade 1
  frameBuffer->addColorTexture(GL_R32F, GL_RED, GL_CLAMP_TO_BORDER, GL_TEXTURE5);   // Shadowcaster light view buffer, cascade 2
  frameBuffer->addDepthStencilBuffer();
  frameBuffer->bindColorTextures();
}

void SBuffer::createShaderPrograms() {
  VertexShaderInput geometryInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  // Light view program
  lightViewProgram.create();
  lightViewProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/lightview.vertex.glsl"));
  lightViewProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/lightview.fragment.glsl"));
  lightViewProgram.link();
  lightViewProgram.use();
  lightViewProgram.setVertexInputs<float>(5, geometryInputs);
}

ShaderProgram& SBuffer::getLightViewProgram() {
  return lightViewProgram;
}

void SBuffer::useFirstShadowCascade() {
  GLenum targets[] = {
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2
  };

  frameBuffer->transferColorTexture(GL_COLOR_ATTACHMENT0, targets, 2);
}

void SBuffer::writeToShadowCascade(unsigned int cascadeIndex) {
  frameBuffer->bindColorTexture(GL_COLOR_ATTACHMENT0 + cascadeIndex);
}