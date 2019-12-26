#include <algorithm>

#include "subsystem/Stage.h"

Stage::~Stage() {
  for (auto* object : objects) {
    delete object;
  }

  objects.clear();
}

void Stage::add(Entity* entity) {
  if (entity->isOfType<Object>()) {
    objects.push_back((Object*)entity);
  }

  if (entityAddedHandler) {
    entityAddedHandler(entity);
  }
}

const std::vector<Object*>& Stage::getObjects() const {
  return objects;
}

void Stage::onEntityAdded(EntityHandler handler) {
  entityAddedHandler = handler;
}

void Stage::onEntityRemoved(EntityHandler handler) {
  entityRemovedHandler = handler;
}

void Stage::remove(Entity* entity) {
  if (entity->isOfType<Object>()) {
    objects.erase(std::remove(objects.begin(), objects.end(), entity), objects.end());
  }

  if (entityRemovedHandler) {
    entityRemovedHandler(entity);
  }
}