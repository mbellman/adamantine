#include "DefaultScene.h"
#include "subsystem/Entities.h"

void DefaultScene::onInit() {
  auto* cube = create<Cube>();

  cube->setPosition({ 0, 0, -50.0f });
  cube->setScale(5.0f);

  entities.add(cube);
}