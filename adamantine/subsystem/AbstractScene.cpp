#include "AbstractScene.h"

const Camera& AbstractScene::getCamera() const {
  return camera;
}

const EntityContainer& AbstractScene::getEntityContainer() const {
  return entities;
}

bool AbstractScene::isActive() const {
  return inputSystem.didCloseWindow();
}

void AbstractScene::onInit() {}

void AbstractScene::onUpdate(int dt) {}

void AbstractScene::pollInput() {
  inputSystem.poll();
}