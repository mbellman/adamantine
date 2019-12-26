#pragma once

#include <vector>
#include <functional>

#include "subsystem/Entities.h"

typedef std::function<void(Entity*)> EntityHandler;

class Stage {
public:
  ~Stage();

  void add(Entity* entity);
  const std::vector<Object*>& getObjects() const;
  void onEntityAdded(EntityHandler handler);
  void onEntityRemoved(EntityHandler handler);
  void remove(Entity* entity);

private:
  std::vector<Object*> objects;
  EntityHandler entityAddedHandler = nullptr;
  EntityHandler entityRemovedHandler = nullptr;
};