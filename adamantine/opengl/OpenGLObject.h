#pragma once

#include <map>
#include <string>

#include "subsystem/Entities.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/OpenGLTexture.h"
#include "opengl/ShaderProgram.h"

class OpenGLObject {
public:
  OpenGLObject(const Object* object);

  static void freeCachedResources();

  void bind();
  ShaderProgram* getCustomShader() const;
  const Object* getSourceObject() const;
  bool hasNormalMap() const;
  bool hasCustomShader() const;
  bool hasTexture() const;
  void render();

private:
  static std::map<int, OpenGLTexture*> textureMap;
  static std::map<int, OpenGLPipeline*> pipelineMap;
  static std::map<std::string, ShaderProgram*> shaderMap;

  const Object* sourceObject = nullptr;
  OpenGLPipeline* glPipeline = nullptr;
  OpenGLTexture* glTexture = nullptr;
  OpenGLTexture* glNormalMap = nullptr;
  ShaderProgram* customShader = nullptr;

  static OpenGLPipeline* createOpenGLPipeline(const Object* object);
  static OpenGLTexture* createOpenGLTexture(const Texture* texture, GLenum unit);
  static ShaderProgram* createShaderProgram(std::string fragShaderPath);
};