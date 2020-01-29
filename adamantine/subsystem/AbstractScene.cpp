#include "subsystem/AbstractScene.h"
#include "subsystem/RNG.h"

#include <cstdio>

AbstractScene::AbstractScene() {
  RNG::seed();
}

const Camera& AbstractScene::getCamera() const {
  return camera;
}

InputSystem& AbstractScene::getInputSystem() {
  return inputSystem;
}

float AbstractScene::getRunningTime() {
  return runningTime;
}

const Stage& AbstractScene::getStage() const {
  return stage;
}

void AbstractScene::onEntityAdded(EntityHandler handler) {
  stage.onEntityAdded(handler);
}

void AbstractScene::onEntityRemoved(EntityHandler handler) {
  stage.onEntityRemoved(handler);
}

void AbstractScene::onInit() {}

void AbstractScene::onUpdate(float dt) {}

void AbstractScene::update(float dt) {
  onUpdate(dt);

  for (auto* object : stage.getObjects()) {
    if (object->onUpdate) {
      object->onUpdate(dt);
    }
  }

  for (auto* light : stage.getLights()) {
    if (light->onUpdate) {
      light->onUpdate(dt);
    }
  }

  runningTime += dt;
}