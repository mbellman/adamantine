#pragma once

#include "subsystem/AbstractScene.h"

class DefaultScene : public AbstractScene {
public:
  void onInit() override;
  void onUpdate(int dt) override;
};