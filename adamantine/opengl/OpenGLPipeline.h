#pragma once

#include "glew.h"
#include "glut.h"
#include "subsystem/Entities.h"

class OpenGLPipeline {
public:
  OpenGLPipeline();

  void bind();
  void createFromObject(const Object* object);
  void pipe(int size, float* buffer);
  void render();
  void setTotalVertices(int totalVertices);

private:
  GLuint vao;
  GLuint vbo;
  int totalVertices = 0;
};