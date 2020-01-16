#include "opengl/OpenGLObject.h"
#include "subsystem/Entities.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"

OpenGLObject::OpenGLObject(const Object* object) {
  sourceObject = object;
  glPipeline = new OpenGLPipeline();

  glPipeline->createFromObject(object);

  if (object->texture != nullptr) {
    glTexture = getOpenGLTexture(object->texture);
  }
}

OpenGLObject::~OpenGLObject() {
  delete glPipeline;
}

void OpenGLObject::bind() {
  glPipeline->bind();

  if (glTexture != nullptr) {
    glTexture->use();
  }
}

void OpenGLObject::freeTextureCache() {
  for (auto [ key, glTexture ] : textureMap) {
    delete glTexture;
  }

  textureMap.clear();
}

OpenGLTexture* OpenGLObject::getOpenGLTexture(const Texture* texture) {
  int id = texture->getId();

  if (textureMap.find(id) != textureMap.end()) {
    return textureMap.at(id);
  } else {
    auto* openGLTexture = new OpenGLTexture(texture);

    textureMap.emplace(id, openGLTexture);

    return openGLTexture;
  }
}

const Object* OpenGLObject::getSourceObject() const {
  return sourceObject;
}

bool OpenGLObject::hasTexture() const {
  return glTexture != nullptr;
}

void OpenGLObject::render() {
  if (glTexture != nullptr) {
    glTexture->use();
  }

  glPipeline->render();
}

std::map<int, OpenGLTexture*> OpenGLObject::textureMap;