#include <cmath>

#include "PalaceScene.h"
#include "subsystem/Texture.h"
#include "subsystem/Entities.h"
#include "subsystem/Math.h"

void PalaceScene::addLights() {
  stage.add<Light>([](auto* light) {
    light->type = Light::LightType::DIRECTIONAL;
    light->color = Vec3f(0.1f, 0.5f, 1.0f);
    light->direction = Vec3f(0.5f, -0.5f, 1.0f);
    light->canCastShadows = true;
  });

  stage.add<Light>([](auto* light) {
    light->type = Light::LightType::DIRECTIONAL;
    light->color = Vec3f(1.0f, 0.5f, 1.0f);
    light->direction = Vec3f(-0.5f, -1.0f, 0.5f);
    light->canCastShadows = true;
  });

  ObjLoader ballObj("./demo/ball.obj");

  auto* ball = new Model();

  ball->from(ballObj);
  ball->setColor(Vec3f(1.0f, 0.8f, 0.0f));

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 8; j++) {
      constexpr static float TAU = 3.141592f * 2.0f;
      float p = j / 8.0f;
      Vec3f center = Vec3f(0.0f, (i + 1) * 30.0f, 300.0f);
      Vec3f position = center + Vec3f(sinf(p * TAU), 0.0f, cosf(p * TAU)) * (110.0f - i * 7.0f);

      stage.add<Light>([=](auto* light) {
        light->color = Vec3f(1.0f, 0.8f, 0.1f);
        light->position = position;
        light->radius = 150.0f;
      });

      stage.add<Model>([=](Model* lightBall) {
        lightBall->from(ball);
        lightBall->setPosition(position);
        lightBall->setScale(2.0f);
        lightBall->isEmissive = true;
      });
    }
  }
}

void PalaceScene::addObjects() {
  stage.add<Skybox>([=](Skybox* skybox) {
    skybox->from(assets.createTexture("./demo/clouds-skybox.png"));
    skybox->setScale(5000.0f);

    skybox->onUpdate = [=](float dt) {
      skybox->setPosition(camera.position);
    };
  });

  ObjLoader pagodaObj("./demo/pagoda.obj");

  stage.add<Model>([&](Model* pagoda) {
    pagoda->from(pagodaObj);
    pagoda->setScale(100.0f);
    pagoda->setPosition({ 0.0f, 0.0f, 300.0f });
    pagoda->setColor(Vec3f(0.5f));
    pagoda->texture = assets.createTexture("./demo/pagoda-texture.png");
  });

  stage.add<Mesh>([](Mesh* mesh) {
    mesh->create(4, 4, 100.0f);
    mesh->setColor(Vec3f(0.2f));
    mesh->setPosition({ 0.0f, -50.0f, 300.0f });
  });
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