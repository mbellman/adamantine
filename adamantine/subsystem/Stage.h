#pragma once

#include <vector>
#include <functional>

#include "subsystem/Entities.h"
#include "subsystem/HeapList.h"

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

  const HeapList<Light>& getLights() const;
  const HeapList<Object>& getObjects() const;
  int getTotalShadowCasters() const;
  void onEntityAdded(EntityHandler handler);
  void onEntityRemoved(EntityHandler handler);
  void remove(Entity* entity);
  void removeExpiredEntities();

private:
  HeapList<Object> objects;
  HeapList<Light> lights;
  EntityHandler entityAddedHandler = nullptr;
  EntityHandler entityRemovedHandler = nullptr;
};