#include "AbstractScene.h"

const Camera& AbstractScene::getCamera() const {
  return camera;
}

const EntityContainer& AbstractScene::getEntityContainer() const {
  return entities;
}

void AbstractScene::onInit() {}

void AbstractScene::onUpdate(int dt) {}