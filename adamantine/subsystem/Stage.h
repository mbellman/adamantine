#pragma once

#include <vector>
#include <functional>

#include "subsystem/Entities.h"

typedef std::function<void(Entity*)> EntityHandler;

class Stage {
public:
  ~Stage();

  void add(Entity* entity);

  template<typename T>
  void add(std::function<void(T*)> handler) {
    T* entity = new T();

    handler(entity);
    add(entity);
  }

  const std::vector<Light*>& getLights() const;
  const std::vector<Object*>& getObjects() const;
  int getTotalShadowCasters() const;
  void onEntityAdded(EntityHandler handler);
  void onEntityRemoved(EntityHandler handler);
  void remove(Entity* entity);

private:
  std::vector<Object*> objects;
  std::vector<Light*> lights;
  EntityHandler entityAddedHandler = nullptr;
  EntityHandler entityRemovedHandler = nullptr;
};