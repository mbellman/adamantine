#pragma once

#include <vector>

#include "Entities.h"

class Scene {
public:
  ~Scene();

  void addObject(Object* object);
  const Camera& getCamera() const;
  const std::vector<Object*>& getObjects() const;

private:
  Camera camera;
  std::vector<Object*> objects;
};