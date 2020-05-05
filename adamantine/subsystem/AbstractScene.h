#pragma once

#include <vector>

#include "subsystem/Stage.h"
#include "subsystem/AssetCache.h"
#include "subsystem/Entities.h"
#include "subsystem/InputSystem.h"

class AbstractScene {
public:
  AbstractScene();
  virtual ~AbstractScene() {};

  const Camera& getCamera() const;
  virtual InputSystem& getInputSystem() final;
  float getRunningTime();
  virtual const Stage& getStage() const final;
  void onEntityAdded(EntityHandler handler);
  void onEntityRemoved(EntityHandler handler);
  virtual void onInit();
  virtual void onUpdate(float dt);
  virtual void update(float dt) final;

protected:
  Stage stage;
  AssetCache assets;
  InputSystem inputSystem;
  Camera camera;

private:
  float runningTime = 0.0f;
};