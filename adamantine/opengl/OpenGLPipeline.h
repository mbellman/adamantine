#pragma once

#include "glew.h"
#include "glut.h"
#include "subsystem/Entities.h"

class OpenGLPipeline {
public:
  OpenGLPipeline();

  void createFromObject(const Object* object);
  void render();
  void useVAO();
  void useVBO();

private:
  GLuint vao;
  GLuint vbo;
  int totalVertices = 0;
};