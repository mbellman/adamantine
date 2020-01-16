#pragma once

#include <map>

#include "subsystem/Entities.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"

class OpenGLObject {
public:
  OpenGLObject(const Object* object);
  ~OpenGLObject();

  static void freeTextureCache();

  void bind();
  const Object* getSourceObject() const;
  bool hasTexture() const;
  void render();

private:
  static std::map<int, OpenGLTexture*> textureMap;

  const Object* sourceObject = nullptr;
  OpenGLPipeline* glPipeline = nullptr;
  OpenGLTexture* glTexture = nullptr;

  OpenGLTexture* getOpenGLTexture(const Texture* texture);
};