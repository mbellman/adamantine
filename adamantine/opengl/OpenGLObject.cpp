#include "opengl/OpenGLObject.h"
#include "subsystem/Entities.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"

OpenGLObject::OpenGLObject(const Object* object) {
  sourceObject = object;
  glPipeline = new OpenGLPipeline();

  glPipeline->createFromObject(object);

  if (object->texture != nullptr) {
    glTexture = OpenGLObject::getOpenGLTexture(object->texture, GL_TEXTURE0 + 4);
  }

  if (object->normalMap != nullptr) {
    glNormalMap = OpenGLObject::getOpenGLTexture(object->normalMap, GL_TEXTURE0 + 5);
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

  if (glNormalMap != nullptr) {
    glNormalMap->use();
  }
}

void OpenGLObject::freeTextureCache() {
  for (auto [ key, glTexture ] : textureMap) {
    delete glTexture;
  }

  textureMap.clear();
}

OpenGLTexture* OpenGLObject::getOpenGLTexture(const Texture* texture, GLenum unit) {
  int id = texture->getId();

  if (OpenGLObject::textureMap.find(id) != OpenGLObject::textureMap.end()) {
    return OpenGLObject::textureMap.at(id);
  } else {
    auto* openGLTexture = new OpenGLTexture(texture, unit);

    OpenGLObject::textureMap.emplace(id, openGLTexture);

    return openGLTexture;
  }
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

std::map<int, OpenGLTexture*> OpenGLObject::textureMap;