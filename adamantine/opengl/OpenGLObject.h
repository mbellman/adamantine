#pragma once

#include <map>

#include "subsystem/Entities.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"

class OpenGLObject {
public:
  OpenGLObject(const Object* object);

  static void freeCachedResources();

  void bind();
  const Object* getSourceObject() const;
  bool hasNormalMap() const;
  bool hasTexture() const;
  void render();

private:
  static std::map<int, OpenGLTexture*> textureMap;
  static std::map<int, OpenGLPipeline*> pipelineMap;

  const Object* sourceObject = nullptr;
  OpenGLPipeline* glPipeline = nullptr;
  OpenGLTexture* glTexture = nullptr;
  OpenGLTexture* glNormalMap = nullptr;

  static OpenGLPipeline* createOpenGLPipeline(const Object* object);
  static OpenGLTexture* createOpenGLTexture(const Texture* texture, GLenum unit);
};