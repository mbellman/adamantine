#include "StressTestScene.h"
#include "subsystem/Entities.h"
#include "subsystem/ObjLoader.h"
#include "subsystem/Math.h"
#include "subsystem/RNG.h"

void StressTestScene::addObjects() {
  auto* sandNormalMap = new Texture("./demo/sand-normal-map.png");
  auto* skyTexture = new Texture("./demo/sea-skybox.png");

  assets.addTexture(sandNormalMap);
  assets.addTexture(skyTexture);

  auto* skybox = new Skybox(skyTexture, 5000.0f);

  skybox->onUpdate = [=](float dt) {
    skybox->setPosition(camera.position);
  };

  stage.add(skybox);

  ObjLoader bunnyObj("./demo/bunny-low-res.obj");

  auto* referenceBunny = new Model(bunnyObj);

  referenceBunny->setColor(Vec3f(1.0f, 0.75f, 0.1f));

  for (int x = -3; x <= 3; x++) {
    for (int z = -3; z <= 3; z++) {
      Vec3f bunnyPosition = Vec3f(x * 100.0f, 0.0f, z * 100.0f) + Vec3f(0.0f, 0.0f, 750.0f);
      Vec3f lightPosition = bunnyPosition + Vec3f(0.0f, 100.0f, 0.0f);

      auto* bunny = new Model(referenceBunny);

      bunny->setPosition(bunnyPosition);
      bunny->setScale(20.0f);

      bunny->onUpdate = [=](float dt) {
        bunny->rotate({ 0.0f, dt, 0.0f });
      };

      stage.add(bunny);
    }
  }

  ObjLoader terrainObj("./demo/terrain.obj");

  auto* terrain = new Model(terrainObj);

  terrain->setColor({ 0.1f, 1.0f, 0.15f });
  terrain->setPosition(Vec3f(0.0f, -300.0f, 750.0f));
  terrain->setScale(1000.0f);

  stage.add(terrain);
}

void StressTestScene::addLights() {
  auto* light = new Light();

  light->type = Light::LightType::DIRECTIONAL;
  light->direction = Vec3f(1.0f, -1.0f, 0.5f);
  light->color = Vec3f(1.0f, 0.25f, 0.5f);
  light->canCastShadows = true;

  stage.add(light);
}

void StressTestScene::onInit() {
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

void StressTestScene::onUpdate(float dt) {
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
