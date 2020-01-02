#include <cmath>

#include "DefaultScene.h"
#include "subsystem/Entities.h"
#include "subsystem/RNG.h"

void DefaultScene::onInit() {
  auto* mesh = new Mesh(100, 100, 10.0f);

  mesh->setPosition({ 0.0f, -20.0f, 0.0f });

  mesh->defineOffsets([=](Vec3f& position, int x, int y) {
    position.y = RNG::random() * 5.0f;
  });

  auto* cube1 = new Cube();

  cube1->setScale(8.0f);
  cube1->setPosition({ -25.0f, 10.0f, 50.0f });
  cube1->setOrientation({ 1.5f, 0.7f, 2.1f });

  auto* cube2 = new Cube();

  cube2->setScale(4.0f);
  cube2->setPosition({ 0.0f, 7.0f, 50.0f });
  cube2->setOrientation({ 0.3f, 1.1f, 0.8f });

  auto* cube3 = new Cube();

  cube3->setScale(2.0f);
  cube3->setPosition({ 15.0f, 4.0f, 50.0f });
  cube3->setOrientation({ 0.9f, 2.5f, 3.1f });

  stage.add(mesh);
  stage.add(cube1);
  stage.add(cube2);
  stage.add(cube3);

  inputSystem.onMouseMotion([=](const SDL_MouseMotionEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      camera.orientation.x += event.yrel / 1000.0f;
      camera.orientation.y += event.xrel / 1000.0f;
    }
  });

  inputSystem.onMouseButton([=](const SDL_MouseButtonEvent& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      SDL_SetRelativeMouseMode(SDL_TRUE);
    }
  });

  inputSystem.onKeyDown([=](const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_ESCAPE) {
      SDL_SetRelativeMouseMode(SDL_FALSE);
    }
  });
}

void DefaultScene::onUpdate(float dt) {
  if (inputSystem.isKeyHeld(Key::W)) {
    camera.position += camera.getDirection() * 100.0f * dt;
  }

  if (inputSystem.isKeyHeld(Key::A)) {
    camera.position += camera.getLeftDirection() * 100.0f * dt;
  }

  if (inputSystem.isKeyHeld(Key::S)) {
    camera.position -= camera.getDirection() * 100.0f * dt;
  }

  if (inputSystem.isKeyHeld(Key::D)) {
    camera.position += camera.getRightDirection() * 100.0f * dt;
  }
}