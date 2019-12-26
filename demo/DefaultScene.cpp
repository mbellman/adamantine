#include <cmath>

#include "DefaultScene.h"
#include "subsystem/Entities.h"

void DefaultScene::onInit() {
  for (int i = 0; i < 10000; i++) {
    auto* cube = new Cube();
    float r = 50.0f + i * 2.0f;
    float x = r * sinf(i * 0.1f);
    float z = r * cosf(i * 0.1f);

    cube->setPosition({ x, 0.0f, z });
    cube->setScale(5.0f);

    stage.add(cube);
  }

  auto* mesh = new Mesh(100, 100, 10.0f);

  mesh->setPosition({ -500.0f, -50.0f, 500.0f });

  stage.add(mesh);

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

void DefaultScene::onUpdate(int dt) {
  if (inputSystem.isKeyHeld(Key::W)) {
    camera.position += camera.getDirection() * 2.0f;
  }

  if (inputSystem.isKeyHeld(Key::A)) {
    camera.position += camera.getLeftDirection() * 2.0f;
  }

  if (inputSystem.isKeyHeld(Key::S)) {
    camera.position -= camera.getDirection() * 2.0f;
  }

  if (inputSystem.isKeyHeld(Key::D)) {
    camera.position += camera.getRightDirection() * 2.0f;
  }
}