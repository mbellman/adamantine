#include <cmath>

#include "LightRoomScene.h"
#include "subsystem/Entities.h"
#include "subsystem/Texture.h"
#include "subsystem/RNG.h"

void LightRoomScene::addLights() {
  // for (int i = 0; i < 10; i++) {
  //   auto* light = new Light();

  //   Vec3f position = Vec3f(
  //     RNG::random() - 0.5f,
  //     RNG::random(),
  //     RNG::random() - 0.5f
  //   ).unit() * 150.0f;

  //   light->color = Vec3f(RNG::random(), 0.0f, RNG::random());
  //   light->position = position;
  //   light->radius = 1000.0f;

  //   stage.add(light);
  // }

  auto* cameraLight = new Light();

  cameraLight->type = Light::LightType::SPOTLIGHT;
  cameraLight->color = Vec3f(1.0f);
  cameraLight->direction = Vec3f(0.2f, 0.0f, 1.0f);
  cameraLight->position = Vec3f(-20.0f, 20.0f, 0.0f);
  cameraLight->radius = 1000.0f;
  cameraLight->canCastShadows = true;

  cameraLight->onUpdate = [=](float dt) {
    cameraLight->position = camera.position + camera.getDirection() * 25.0f;
    cameraLight->direction = camera.getDirection();
  };

  stage.add(cameraLight);

  for (int i = 0; i < 10; i++) {
    auto* light = new Light();

    Vec3f basePosition = {
      RNG::random() * 400.0f - 200.0f,
      100.0f,
      RNG::random() * 400.0f - 200.0f
    };

    light->type = Light::LightType::SPOTLIGHT;
    light->color = Vec3f(1.0f, RNG::random(), 0.0f);
    light->radius = 1500.0f;
    light->canCastShadows = true;

    light->onUpdate = [=](float dt) {
      Vec3f direction = Vec3f(sinf(getRunningTime()), -1.0f, cosf(getRunningTime()));

      light->direction = direction;
      light->position = basePosition + direction.unit() * 25.0f;
    };

    stage.add(light);
  }
}

void LightRoomScene::addObjects() {
  constexpr float PI = 3.141592;

  auto* bottom = new Mesh(1, 1, 400.0f);
  auto* top = new Mesh(1, 1, 400.0f);
  auto* left = new Mesh(1, 1, 400.0f);
  auto* right = new Mesh(1, 1, 400.0f);
  auto* front = new Mesh(1, 1, 400.0f);
  auto* back = new Mesh(1, 1, 400.0f);

  bottom->setPosition({ 0.0f, 0.0f, 0.0f });

  top->rotate({ PI, 0.0f, 0.0f });
  top->setPosition({ 0.0f, 400.0f, 0.0f });

  left->rotate({ 0.0f, 0.0f, -PI * 0.5f });
  left->setPosition({ -200.0f, 200.0f, 0.0f });

  right->rotate({ 0.0f, 0.0f, PI * 0.5f });
  right->setPosition({ 200.0f, 200.0f, 0.0f });

  front->rotate({ PI * 0.5f, 0.0f, 0.0f });
  front->setPosition({ 0.0f, 200.0f, 200.0f });

  back->rotate({ -PI * 0.5f, 0.0f, 0.0f });
  back->setPosition({ 0.0f, 200.0f, -200.0f });

  stage.add(bottom);
  stage.add(top);
  stage.add(left);
  stage.add(right);
  stage.add(front);
  stage.add(back);

  ObjLoader lanternObj("./demo/lantern.obj");

  auto* lanternTexture = new Texture("./demo/lantern-texture.png");
  auto* lanternNormalMap = new Texture("./demo/lantern-normal-map.png");

  assets.addTexture(lanternTexture);
  assets.addTexture(lanternNormalMap);

  for (int i = 0; i < 6; i++) {
    float x = sinf((float)i * PI / 3.0f);
    float z = cosf((float)i * PI / 3.0f);

    Vec3f position = Vec3f(x, 0.0f, z) * 100.0f;

    auto* lantern = new Model(lanternObj);

    lantern->setPosition(position);
    lantern->setScale(50.0f);
    lantern->setColor(Vec3f(0.5f));
    lantern->texture = lanternTexture;
    lantern->normalMap = lanternNormalMap;

    stage.add(lantern);

    if (i % 2 == 0) {
      auto* light = new Light();

      light->color = Vec3f(1.0f, 0.75f, 0.1f);
      light->position = position + Vec3f(0.0f, 35.0f, 0.0f);
      light->radius = 500.0f;

      auto* lightCube = new Cube();

      lightCube->setColor(Vec3f(1.0f));
      lightCube->setPosition(light->position);
      lightCube->setScale(2.0f);
      lightCube->isEmissive = true;

      stage.add(light);
      stage.add(lightCube);
    }
  }
}

void LightRoomScene::onInit() {
  addLights();
  addObjects();

  camera.position = Vec3f(0.0f, 100.0f, 0.0f);

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

void LightRoomScene::onUpdate(float dt) {
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