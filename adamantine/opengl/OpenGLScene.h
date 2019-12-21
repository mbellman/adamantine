#pragma once

#include <vector>

#include "subsystem/Entities.h"
#include "subsystem/AbstractScene.h"
#include "opengl/VertexPipeline.h"

struct OpenGLObject : public Object {
  ~OpenGLObject();

  VertexPipeline* pipeline;
};

class OpenGLScene : public AbstractScene {
protected:
  template<typename T>
  OpenGLObject* create() {
    auto* object = (OpenGLObject*)(new T());

    object->pipeline = createVertexPipeline(object);

    return object;
  }

private:
  VertexPipeline* createVertexPipeline(Object* object);
};