#pragma once

#include <vector>

#include "subsystem/Entities.h"
#include "subsystem/EntityContainer.h"
#include "subsystem/InputSystem.h"

class AbstractScene {
public:
  virtual ~AbstractScene() {};

  const Camera& getCamera() const;
  const EntityContainer& getEntityContainer() const;
  bool isActive() const;
  virtual void onInit();
  virtual void onUpdate(int dt);
  virtual void pollInput() final;

protected:
  EntityContainer entities;
  Camera camera;
  InputSystem inputSystem;
};