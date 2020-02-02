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
  } else if (entity->isOfType<Light>()) {
    lights.push_back((Light*)entity);
  }

  if (entityAddedHandler) {
    entityAddedHandler(entity);
  }
}

const std::vector<Light*>& Stage::getLights() const {
  return lights;
}

const std::vector<Object*>& Stage::getObjects() const {
  return objects;
}

int Stage::getTotalShadowCasters() const {
  int total = 0;

  for (auto* light : lights) {
    if (light->canCastShadows) {
      total++;
    }
  }

  return total;
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
  } else if (entity->isOfType<Light>()) {
    lights.erase(std::remove(lights.begin(), lights.end(), entity), lights.end());
  }

  if (entityRemovedHandler) {
    entityRemovedHandler(entity);
  }
}