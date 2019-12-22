#pragma once

#include <vector>

#include "subsystem/Entities.h"
#include "subsystem/AbstractScene.h"
#include "opengl/VertexPipeline.h"

class OpenGLObject : public Object {
public:
  ~OpenGLObject();

  void draw();
  void initialize();
  void use();

private:
  VertexPipeline* pipeline = nullptr;
};

class OpenGLScene : public AbstractScene {
protected:
  template<typename T>
  OpenGLObject* create() {
    auto* object = (OpenGLObject*)(new T());

    object->initialize();

    return object;
  }
};