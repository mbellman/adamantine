#include "Scene.h"

Scene::~Scene() {
  for (auto* object : objects) {
    delete object;
  }

  objects.clear();
}

void Scene::addObject(Object* object) {
  objects.push_back(object);
}

const Camera& Scene::getCamera() const {
  return camera;
}

const std::vector<Object*>& Scene::getObjects() const {
  return objects;
}