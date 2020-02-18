#include <cmath>

#include "PalaceScene.h"
#include "subsystem/Texture.h"
#include "subsystem/Entities.h"
#include "subsystem/Math.h"

void PalaceScene::addLights() {
  auto* light = new Light();

  light->type = Light::LightType::DIRECTIONAL;
  light->color = Vec3f(0.1f, 0.5f, 1.0f);
  light->direction = Vec3f(0.5f, -0.5f, 1.0f);
  light->canCastShadows = true;

  auto* light2 = new Light();

  light2->type = Light::LightType::DIRECTIONAL;
  light2->color = Vec3f(1.0f, 0.5f, 1.0f);
  light2->direction = Vec3f(-0.5f, -1.0f, 0.5f);
  light2->canCastShadows = true;

  stage.add(light);
  stage.add(light2);

  ObjLoader ballObj("./demo/ball.obj");

  auto* ball = new Model(ballObj);

  ball->setColor(Vec3f(1.0f, 0.8f, 0.0f));

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 8; j++) {
      constexpr static float TAU = 3.141592f * 2.0f;
      auto* light = new Light();
      float p = j / 8.0f;
      Vec3f center = Vec3f(0.0f, (i + 1) * 30.0f, 300.0f);

      light->color = Vec3f(1.0f, 0.8f, 0.1f);
      light->position = center + Vec3f(sinf(p * TAU), 0.0f, cosf(p * TAU)) * (110.0f - i * 7.0f);
      light->radius = 150.0f;

      auto* lightBall = new Model(ball);

      lightBall->setPosition(light->position);
      lightBall->setScale(2.0f);
      lightBall->isEmissive = true;

      stage.add(light);
      stage.add(lightBall);
    }
  }
}

void PalaceScene::addObjects() {
  auto* skyTexture = new Texture("./demo/clouds-skybox.png");
  auto* skybox = new Skybox(skyTexture, 5000.0f);

  skybox->onUpdate = [=](float dt) {
    skybox->setPosition(camera.position);
  };

  ObjLoader pagodaObj("./demo/pagoda.obj");

  auto* pagodaTexture = new Texture("./demo/pagoda-texture.png");
  auto* pagoda = new Model(pagodaObj);

  pagoda->setScale(100.0f);
  pagoda->setPosition({ 0.0f, 0.0f, 300.0f });
  pagoda->setColor(Vec3f(0.5f));
  pagoda->texture = pagodaTexture;

  assets.addTexture(pagodaTexture);

  auto* mesh = new Mesh(4, 4, 100.0f);

  mesh->setColor(Vec3f(0.2f));
  mesh->setPosition({ 0.0f, -50.0f, 300.0f });

  stage.add(skybox);
  stage.add(pagoda);
  stage.add(mesh);
  assets.addTexture(skyTexture);
}

void PalaceScene::onInit() {
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

void PalaceScene::onUpdate(float dt) {
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