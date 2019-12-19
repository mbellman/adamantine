#pragma once

#include <vector>

#include "Entities.h"
#include "VertexPipeline.h"

struct OpenGLObject {
  OpenGLObject(Object* baseObject, VertexPipeline* pipeline) : baseObject(baseObject), pipeline(pipeline) {};
  ~OpenGLObject();

  Object* baseObject;
  VertexPipeline* pipeline;
};

class OpenGLScene {
public:
  ~OpenGLScene();

  void addObject(Object* object);
  const Camera& getCamera() const;
  const std::vector<OpenGLObject*>& getOpenGLObjects() const;

private:
  Camera camera;
  std::vector<OpenGLObject*> openGLObjects;
};