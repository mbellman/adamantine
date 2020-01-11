#pragma once

#include <vector>

#include "subsystem/Entities.h"
#include "subsystem/Stage.h"
#include "subsystem/InputSystem.h"

struct SceneSettings {
  Vec3f backgroundColor;
};

class AbstractScene {
public:
  AbstractScene();
  virtual ~AbstractScene() {};

  const Camera& getCamera() const;
  virtual InputSystem& getInputSystem() final;
  virtual const SceneSettings& getSettings() const final;
  virtual const Stage& getStage() const final;
  void onEntityAdded(EntityHandler handler);
  void onEntityRemoved(EntityHandler handler);
  virtual void onInit();
  virtual void onUpdate(float dt);
  virtual void updateRunningTime(float dt) final;

protected:
  Stage stage;
  Camera camera;
  InputSystem inputSystem;
  SceneSettings settings;

  float getRunningTime();

private:
  float runningTime = 0.0f;
};