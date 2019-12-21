#include "DefaultScene.h"
#include "subsystem/Entities.h"

#include <cmath>

void DefaultScene::onInit() {
  for (int i = 0; i < 10000; i++) {
    auto* cube = create<Cube>();
    float r = 50.0f + i * 2.0f;
    float x = r * sinf(i * 0.1f);
    float z = r * cosf(i * 0.1f);

    cube->setPosition({ x, 0.0f, z });
    cube->setScale(5.0f);

    entities.add(cube);
  }

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