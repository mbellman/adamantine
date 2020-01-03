#include "subsystem/AbstractScene.h"
#include "subsystem/RNG.h"

#include <cstdio>

AbstractScene::AbstractScene() {
  RNG::seed();
}

const Camera& AbstractScene::getCamera() const {
  return camera;
}

float AbstractScene::getRunningTime() {
  return runningTime;
}

const SceneSettings& AbstractScene::getSettings() const {
  return settings;
}

const Stage& AbstractScene::getStage() const {
  return stage;
}

bool AbstractScene::isActive() const {
  return inputSystem.didCloseWindow();
}

void AbstractScene::onEntityAdded(EntityHandler handler) {
  stage.onEntityAdded(handler);
}

void AbstractScene::onEntityRemoved(EntityHandler handler) {
  stage.onEntityRemoved(handler);
}

void AbstractScene::onInit() {}

void AbstractScene::onUpdate(float dt) {}

void AbstractScene::pollInput() {
  inputSystem.poll();
}
void AbstractScene::updateRunningTime(float dt) {
  runningTime += dt;
}