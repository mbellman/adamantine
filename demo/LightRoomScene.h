#pragma once

#include "subsystem/AbstractScene.h"
#include "subsystem/Entities.h"

class LightRoomScene : public AbstractScene {
public:
  void onInit() override;
  void onUpdate(float dt) override;

private:
  void addLights();
  void addObjects();

  Light* cameraLight = nullptr;
};