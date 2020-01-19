#pragma once

#include "glew.h"
#include "glut.h"
#include "subsystem/Entities.h"

class OpenGLPipeline {
public:
  OpenGLPipeline();

  void bind();
  void createFromObject(const Object* object);
  void createScreenQuad();
  void pipe(int size, float* buffer);
  void render();

private:
  GLuint vao;
  GLuint vbo;
  int totalVertices = 0;
};