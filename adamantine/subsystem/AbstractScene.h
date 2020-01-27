#pragma once

#include <vector>

#include "subsystem/Stage.h"
#include "subsystem/AssetCache.h"
#include "subsystem/Entities.h"
#include "subsystem/InputSystem.h"

struct SceneSettings {
  Vec3f ambientLightColor;
  Vec3f ambientLightDirection;
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
  virtual void update(float dt) final;

protected:
  Stage stage;
  AssetCache assets;
  InputSystem inputSystem;
  Camera camera;
  SceneSettings settings;

  float getRunningTime();

private:
  float runningTime = 0.0f;
};