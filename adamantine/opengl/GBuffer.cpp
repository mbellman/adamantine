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

void GBuffer::createFrameBuffer(unsigned int width, unsigned int height) {
  if (frameBuffer != nullptr) {
    delete frameBuffer;
  }

  frameBuffer = new FrameBuffer(width, height);

  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB, GL_CLAMP_TO_BORDER, GL_TEXTURE0);     // Color
  frameBuffer->addColorTexture(GL_RGBA32F, GL_RGBA, GL_CLAMP_TO_BORDER, GL_TEXTURE1);   // Normal/depth
  frameBuffer->addColorTexture(GL_RGB32F, GL_RGB, GL_CLAMP_TO_BORDER, GL_TEXTURE2);     // Position
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

ShaderProgram& GBuffer::getShaderProgram(GBuffer::Shader shader) {
  switch (shader) {
    case GBuffer::Shader::GEOMETRY:
      return geometryProgram;
    case GBuffer::Shader::ILLUMINATION:
      return illuminationProgram;
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

void GBuffer::writeToAllBuffers() {
  frameBuffer->bindColorTextures();
}