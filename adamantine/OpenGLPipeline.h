#pragma once

struct OpenGLPipeline {
  void bufferVertices(int size, const float* vertices);
  void createVertexArray();
  void createVertexBuffer();
};