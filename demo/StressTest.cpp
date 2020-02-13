#include "StressTest.h"
#include "subsystem/Entities.h"
#include "subsystem/ObjLoader.h"
#include "subsystem/Math.h"
#include "subsystem/RNG.h"

void StressTest::addObjects() {
  ObjLoader bunnyObj("./demo/bunny-high-res.obj");

  auto* sandNormalMap = new Texture("./demo/sand-normal-map.png");

  assets.addTexture(sandNormalMap);

  for (int x = -3; x <= 3; x++) {
    for (int z = -3; z <= 3; z++) {
      Vec3f bunnyPosition = Vec3f(x * 100.0f, 0.0f, z * 100.0f) + Vec3f(0.0f, 0.0f, 750.0f);
      Vec3f lightPosition = bunnyPosition + Vec3f(0.0f, 100.0f, 0.0f);

      auto* bunny = new Model(bunnyObj);

      bunny->setPosition(bunnyPosition);
      bunny->setScale(30.0f);
      bunny->setColor(Vec3f(1.0f, 0.75f, 0.1f));

      bunny->onUpdate = [=](float dt) {
        bunny->rotate({ 0.0f, dt, 0.0f });
      };

      stage.add(bunny);
    }
  }

  auto* mesh = new Mesh(2, 2, 500.0f);

  mesh->setPosition(Vec3f(0.0f, -25.0f, 750.0f));
  mesh->setColor(Vec3f(1.0f));
  mesh->normalMap = sandNormalMap;

  stage.add(mesh);
}

void StressTest::addLights() {
  auto* light = new Light();

  light->type = Light::LightType::DIRECTIONAL;
  light->direction = Vec3f(1.0f, -1.0f, 0.5f);
  light->color = Vec3f(1.0f, 0.25f, 0.5f);
  light->canCastShadows = true;

  stage.add(light);
}

void StressTest::onInit() {
  addObjects();
  addLights();

  inputSystem.onMouseMotion([=](const SDL_MouseMotionEvent& event) {
    if (SDL_GetRelativeMouseMode()) {
      camera.orientation.x -= event.yrel / 1000.0f;
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

void StressTest::onUpdate(float dt) {
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
