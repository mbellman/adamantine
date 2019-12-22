#include "glew.h"
#include "opengl/OpenGLScene.h"

/**
 * OpenGLObject
 * ------------
 */
OpenGLObject::~OpenGLObject() {
  delete pipeline;
}

void OpenGLObject::draw() {
  use();

  glDrawArrays(GL_TRIANGLES, 0, getPolygons().size() * 3);
}

void OpenGLObject::initialize() {
  int vertexBufferSize = getPolygons().size() * 3 * 6;
  float* vertexBuffer = new float[vertexBufferSize];
  int idx = 0;

  // TODO figure out how to recycle vertices
  for (auto* polygon : getPolygons()) {
    for (int v = 0; v < 3; v++) {
      const Vertex3d& vertex = *polygon->vertices[v];

      vertexBuffer[idx++] = vertex.position.x;
      vertexBuffer[idx++] = vertex.position.y;
      vertexBuffer[idx++] = vertex.position.z;

      vertexBuffer[idx++] = vertex.color.x;
      vertexBuffer[idx++] = vertex.color.y;
      vertexBuffer[idx++] = vertex.color.z;
    }
  }

  pipeline = new VertexPipeline();

  pipeline->bufferVertices(vertexBufferSize * sizeof(float), vertexBuffer);

  delete[] vertexBuffer;
}

void OpenGLObject::use() {
  pipeline->useVAO();
}