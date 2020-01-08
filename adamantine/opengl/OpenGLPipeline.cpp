#include "glew.h"
#include "glut.h"
#include "opengl/OpenGLPipeline.h"

OpenGLPipeline::OpenGLPipeline() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  bind();
}

void OpenGLPipeline::createFromObject(const Object* object) {
  // Store the number of vertices for subsequent draw calls
  totalVertices = object->getPolygons().size() * 3;

  int bufferSize = totalVertices * 11;
  float* buffer = new float[bufferSize];
  int idx = 0;

  // TODO figure out how to recycle vertices
  for (auto* polygon : object->getPolygons()) {
    for (int v = 0; v < 3; v++) {
      const Vertex3d& vertex = *polygon->vertices[v];

      buffer[idx++] = vertex.position.x;
      buffer[idx++] = vertex.position.y;
      buffer[idx++] = vertex.position.z;

      buffer[idx++] = vertex.normal.x;
      buffer[idx++] = vertex.normal.y;
      buffer[idx++] = vertex.normal.z;

      buffer[idx++] = vertex.color.x;
      buffer[idx++] = vertex.color.y;
      buffer[idx++] = vertex.color.z;

      buffer[idx++] = vertex.uv.x;
      buffer[idx++] = vertex.uv.y;
    }
  }

  glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), buffer, GL_STATIC_DRAW);

  delete[] buffer;
}

void OpenGLPipeline::render() {
  bind();

  glDrawArrays(GL_TRIANGLES, 0, totalVertices);
}

void OpenGLPipeline::bind() {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}