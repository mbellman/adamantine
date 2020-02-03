#pragma once

#include "subsystem/AbstractScene.h"

class LightRoomScene : public AbstractScene {
public:
  void onInit() override;
  void onUpdate(float dt) override;

public:
  void addLights();
  void addObjects();
};