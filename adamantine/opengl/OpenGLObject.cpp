#include "opengl/OpenGLObject.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"
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
    pipelineMap.emplace(referenceId, pipeline);

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

void OpenGLObject::freeCachedResources() {
  for (auto [ key, glTexture ] : textureMap) {
    delete glTexture;
  }

  for (auto [ key, glPipeline ] : pipelineMap) {
    delete glPipeline;
  }

  textureMap.clear();
  pipelineMap.clear();
}

const Object* OpenGLObject::getSourceObject() const {
  return sourceObject;
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