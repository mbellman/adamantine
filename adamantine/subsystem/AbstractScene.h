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
  const SceneSettings& getSettings() const;
  const Stage& getStage() const;
  bool isActive() const;
  void onEntityAdded(EntityHandler handler);
  void onEntityRemoved(EntityHandler handler);
  virtual void onInit();
  virtual void onUpdate(float dt);
  virtual void pollInput() final;

protected:
  Stage stage;
  Camera camera;
  InputSystem inputSystem;
  SceneSettings settings;
};