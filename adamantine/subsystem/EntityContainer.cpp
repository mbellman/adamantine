#include <algorithm>

#include "EntityContainer.h"

EntityContainer::~EntityContainer() {
  for (auto* object : objects) {
    delete object;
  }

  objects.clear();
}

void EntityContainer::add(Entity* entity) {
  if (entity->isOfType<Object>()) {
    objects.push_back((Object*)entity);
  }
}

const std::vector<Object*>& EntityContainer::getObjects() const {
  return objects;
}

void EntityContainer::remove(Entity* entity) {
  if (entity->isOfType<Object>()) {
    objects.erase(std::remove(objects.begin(), objects.end(), entity), objects.end());
  }
}