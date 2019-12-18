#pragma once

#include "glew.h"
#include "glut.h"

class VertexPipeline {
public:
  VertexPipeline();

  void bufferVertices(int size, const float* vertices);
  void useArray();
  void useBuffer();

private:
  GLuint vao;
  GLuint vbo;
};