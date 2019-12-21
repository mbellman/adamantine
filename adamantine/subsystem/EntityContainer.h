#pragma once

#include <vector>

#include "subsystem/Entities.h"

class EntityContainer {
public:
  ~EntityContainer();

  void add(Entity* entity);
  const std::vector<Object*>& getObjects() const;
  void remove(Entity* entity);

private:
  std::vector<Object*> objects;
};