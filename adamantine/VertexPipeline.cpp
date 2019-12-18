#include "VertexPipeline.h"
#include "glew.h"
#include "glut.h"

#include <cstdio>

VertexPipeline::VertexPipeline() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VertexPipeline::bufferVertices(int size, const float* vertices) {
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexPipeline::useArray() {
  glBindVertexArray(vao);
}

void VertexPipeline::useBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}