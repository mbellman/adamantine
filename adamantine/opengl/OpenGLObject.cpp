#include "opengl/OpenGLObject.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"
#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"
#include "subsystem/Entities.h"

OpenGLObject::OpenGLObject(const Object* object) {
  sourceObject = object;
  glPipeline = OpenGLObject::createOpenGLPipeline(object);

  if (object->texture != nullptr) {
    glTexture = OpenGLObject::createOpenGLTexture(object->texture, GL_TEXTURE7);
  }

  if (object->normalMap != nullptr) {
    glNormalMap = OpenGLObject::createOpenGLTexture(object->normalMap, GL_TEXTURE8);
  }

  if (object->shader != nullptr) {
    std::string path(object->shader);

    customShader = OpenGLObject::createShaderProgram(path);
  }
}

void OpenGLObject::bind() {
  glPipeline->bind();

  if (glTexture != nullptr) {
    glTexture->use();
  }

  if (glNormalMap != nullptr) {
    glNormalMap->use();
  }
}

OpenGLPipeline* OpenGLObject::createOpenGLPipeline(const Object* object) {
  int referenceId = object->getReference()->id;

  if (OpenGLObject::pipelineMap.find(referenceId) != OpenGLObject::pipelineMap.end()) {
    return OpenGLObject::pipelineMap.at(referenceId);
  } else {
    auto* pipeline = new OpenGLPipeline();

    pipeline->createFromObject(object->getReference());

    OpenGLObject::pipelineMap.emplace(referenceId, pipeline);

    return pipeline;
  }
}

OpenGLTexture* OpenGLObject::createOpenGLTexture(const Texture* texture, GLenum unit) {
  int id = texture->getId();

  if (OpenGLObject::textureMap.find(id) != OpenGLObject::textureMap.end()) {
    return OpenGLObject::textureMap.at(id);
  } else {
    auto* openGLTexture = new OpenGLTexture(texture, unit);

    OpenGLObject::textureMap.emplace(id, openGLTexture);

    return openGLTexture;
  }
}

ShaderProgram* OpenGLObject::createShaderProgram(std::string path) {
  if (OpenGLObject::shaderMap.find(path) != OpenGLObject::shaderMap.end()) {
    return OpenGLObject::shaderMap.at(path);
  } else {
    VertexShaderInput geometryInputs[] = {
      { "vertexPosition", 3, GL_FLOAT },
      { "vertexNormal", 3, GL_FLOAT},
      { "vertexTangent", 3, GL_FLOAT },
      { "vertexColor", 3, GL_FLOAT },
      { "vertexUv", 2, GL_FLOAT }
    };

    auto* program = new ShaderProgram();

    program->create();
    program->attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/geometry.vertex.glsl"));
    program->attachShader(ShaderLoader::loadFragmentShader(path.c_str()));
    program->link();
    program->use();
    program->setVertexInputs<float>(5, geometryInputs);

    OpenGLObject::shaderMap.emplace(path, program);

    return program;
  }
}

void OpenGLObject::freeCachedResources() {
  for (auto [ key, glTexture ] : textureMap) {
    delete glTexture;
  }

  for (auto [ key, glPipeline ] : pipelineMap) {
    delete glPipeline;
  }

  for (auto [ key, shader ] : shaderMap) {
    delete shader;
  }

  textureMap.clear();
  pipelineMap.clear();
  shaderMap.clear();
}

const Object* OpenGLObject::getSourceObject() const {
  return sourceObject;
}

ShaderProgram* OpenGLObject::getCustomShader() const {
  return customShader;
}

bool OpenGLObject::hasCustomShader() const {
  return customShader != nullptr;
}

bool OpenGLObject::hasNormalMap() const {
  return glNormalMap != nullptr;
}

bool OpenGLObject::hasTexture() const {
  return glTexture != nullptr;
}

void OpenGLObject::render() {
  bind();

  glPipeline->render();
}

std::map<int, OpenGLPipeline*> OpenGLObject::pipelineMap;
std::map<int, OpenGLTexture*> OpenGLObject::textureMap;
std::map<std::string, ShaderProgram*> OpenGLObject::shaderMap;