#pragma once

#include "opengl/OpenGLScene.h"

class DefaultScene : public OpenGLScene {
public:
  void onInit() override;
  void onUpdate(int dt) override;
};