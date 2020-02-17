#pragma once

#include "subsystem/AbstractScene.h"

class StressTestScene : public AbstractScene {
public:
  void onInit() override;
  void onUpdate(float dt) override;

private:
  void addLights();
  void addObjects();
};