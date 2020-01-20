#include "glew.h"
#include "glut.h"
#include "opengl/OpenGLPipeline.h"

OpenGLPipeline::OpenGLPipeline() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  bind();
}

void OpenGLPipeline::bind() {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void OpenGLPipeline::createFromObject(const Object* object) {
  totalVertices = object->getPolygons().size() * 3;

  int bufferSize = totalVertices * 14;
  float* buffer = new float[bufferSize];
  int idx = 0;

  for (auto* polygon : object->getPolygons()) {
    for (int v = 0; v < 3; v++) {
      const Vertex3d& vertex = *polygon->vertices[v];

      buffer[idx++] = vertex.position.x;
      buffer[idx++] = vertex.position.y;
      buffer[idx++] = vertex.position.z;

      buffer[idx++] = vertex.normal.x;
      buffer[idx++] = vertex.normal.y;
      buffer[idx++] = vertex.normal.z;

      buffer[idx++] = vertex.tangent.x;
      buffer[idx++] = vertex.tangent.y;
      buffer[idx++] = vertex.tangent.z;

      buffer[idx++] = vertex.color.x;
      buffer[idx++] = vertex.color.y;
      buffer[idx++] = vertex.color.z;

      buffer[idx++] = vertex.uv.x;
      buffer[idx++] = vertex.uv.y;
    }
  }

  pipe(bufferSize, buffer);

  delete[] buffer;
}

void OpenGLPipeline::createScreenQuad() {
  float quad[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f
  };

  pipe(24, quad);

  totalVertices = 6;
}

void OpenGLPipeline::pipe(int size, float* buffer) {
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), buffer, GL_STATIC_DRAW);
}

void OpenGLPipeline::render() {
  bind();

  glDrawArrays(GL_TRIANGLES, 0, totalVertices);
}