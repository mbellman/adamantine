#include "opengl/OpenGLScene.h"

/**
 * OpenGLObject
 * ------------
 */
OpenGLObject::~OpenGLObject() {
  delete baseObject;
  delete pipeline;
}

/**
 * OpenGLScene
 * -----------
 */
OpenGLScene::~OpenGLScene() {
  for (auto* openGLObject : openGLObjects) {
    delete openGLObject;
  }

  openGLObjects.clear();
}

void OpenGLScene::addObject(Object* object) {
  const std::vector<Polygon*>& polygons = object->getPolygons();
  int vertexBufferSize = polygons.size() * 3 * 6;
  float* vertexBuffer = new float[vertexBufferSize];
  int i = 0;

  // TODO figure out how to recycle vertices
  for (auto* polygon : polygons) {
    for (int v = 0; v < 3; v++) {
      const Vertex3d& vertex = *polygon->vertices[v];

      vertexBuffer[i++] = vertex.position.x;
      vertexBuffer[i++] = vertex.position.y;
      vertexBuffer[i++] = vertex.position.z;

      vertexBuffer[i++] = vertex.color.x;
      vertexBuffer[i++] = vertex.color.y;
      vertexBuffer[i++] = vertex.color.z;
    }
  }

  VertexPipeline* pipeline = new VertexPipeline();

  pipeline->bufferVertices(vertexBufferSize * sizeof(float), vertexBuffer);

  delete[] vertexBuffer;

  OpenGLObject* openGLObject = new OpenGLObject(object, pipeline);

  openGLObjects.push_back(openGLObject);
}

const Camera& OpenGLScene::getCamera() const {
  return camera;
}

const std::vector<OpenGLObject*>& OpenGLScene::getOpenGLObjects() const {
  return openGLObjects;
}