#include "OpenGLPipeline.h"
#include "glew.h"
#include "glut.h"

void OpenGLPipeline::bufferVertices(int size, const float* vertices) {
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void OpenGLPipeline::createVertexArray() {
  GLuint vao;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
}

void OpenGLPipeline::createVertexBuffer() {
  GLuint vbo;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}