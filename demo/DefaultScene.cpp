#include "DefaultScene.h"
#include "subsystem/Entities.h"

void DefaultScene::onInit() {
  auto* cube = create<Cube>();

  cube->setPosition({ 0, 0, -50.0f });
  cube->setScale(5.0f);

  entities.add(cube);

  inputSystem.onMouseMotion([=](const SDL_MouseMotionEvent& event) {
    camera.orientation.x += event.yrel / 1000.0f;
    camera.orientation.y += event.xrel / 1000.0f;
  });
}

void DefaultScene::onUpdate(int dt) {
  if (inputSystem.isKeyHeld(Key::W)) {
    camera.position += camera.getDirection() * 0.5f;
  }

  if (inputSystem.isKeyHeld(Key::A)) {
    camera.position += camera.getLeftDirection() * 0.5f;
  }

  if (inputSystem.isKeyHeld(Key::S)) {
    camera.position -= camera.getDirection() * 0.5f;
  }

  if (inputSystem.isKeyHeld(Key::D)) {
    camera.position += camera.getRightDirection() * 0.5f;
  }
}