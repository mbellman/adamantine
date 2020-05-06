#include <cmath>

#include "PalaceScene.h"
#include "subsystem/Texture.h"
#include "subsystem/Entities.h"
#include "subsystem/Math.h"

void PalaceScene::addLights() {
  stage.add<Light>([](auto* light) {
    light->type = Light::LightType::DIRECTIONAL;
    light->color = Vec3f(1.0f, 1.0f, 0.5f);
    light->direction = Vec3f(0.0f, -0.2f, -1.0f);
    light->canCastShadows = true;
  });

  stage.add<Light>([](auto* light) {
    light->type = Light::LightType::DIRECTIONAL;
    light->color = Vec3f(0.1f, 0.5f, 1.0f);
    light->direction = Vec3f(0.0f, -1.0f, -0.5f);
    light->power = 0.5f;
  });

  ObjLoader ballObj("./demo/ball.obj");

  auto* ball = new Model();

  ball->from(ballObj);
  ball->setColor(Vec3f(1.0f, 0.8f, 0.0f));

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 8; j++) {
      constexpr static float TAU = 3.141592f * 2.0f;
      float p = j / 8.0f;
      Vec3f center = Vec3f(0.0f, 50.0f + i * 30.0f, 300.0f);
      Vec3f position = center + Vec3f(sinf(p * TAU), 0.0f, cosf(p * TAU)) * (110.0f - i * 7.0f);

      stage.add<Light>([=](auto* light) {
        light->color = Vec3f(1.0f, 0.8f, 0.1f);
        light->position = position;
        light->radius = 250.0f;
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

  ObjLoader pagodaBaseObj("./demo/pagoda-base.obj");
  ObjLoader pagodaRoofObj("./demo/pagoda-roof.obj");

  auto* pb = new Model();
  auto* pr = new Model();

  pb->from(pagodaBaseObj);
  pr->from(pagodaRoofObj);

  for (int i = 0; i < 4; i++) {
    float y = -30.0f + 50.0f * i;
    float scale = 1.0f - i * 0.1f;
    Vec3f levelPosition = Vec3f(0.0f, y, 300.0f);

    stage.add<Model>([&](Model* pagodaBase) {
      pagodaBase->from(pb);
      pagodaBase->setScale(100.0f * scale);
      pagodaBase->setPosition(levelPosition);
      pagodaBase->setColor(Vec3f(0.5f));
      pagodaBase->texture = assets.createTexture("./demo/base-texture.png");
    });

    stage.add<Model>([&](Model* pagodaRoof) {
      pagodaRoof->from(pr);
      pagodaRoof->setScale(100.0f * scale);
      pagodaRoof->setPosition(levelPosition);
      pagodaRoof->setColor(Vec3f(0.5f));
      pagodaRoof->texture = assets.createTexture("./demo/roof-texture.png");
      pagodaRoof->normalMap = assets.createTexture("./demo/sand-normal-map.png");
    });
  }

  stage.add<Mesh>([](Mesh* mesh) {
    mesh->create(4, 4, 200.0f);
    mesh->setColor(Vec3f(0.2f));
    mesh->setPosition({ 0.0f, -50.0f, 300.0f });
    mesh->shader = "./demo/water.fragment.glsl";
  });

  stage.add<Light>([=](Light* light) {
    Vec3f position = Vec3f(0.0f, 30.0f, 300.0f);

    light->position = position;
    light->color = Vec3f(1.0f, 1.0f, 0.2f);
    light->radius = 1500.0f;
    light->canCastShadows = true;

    light->onUpdate = [=](float dt) {
      light->position = position + Vec3f(
        sinf(getRunningTime()) * 170.0f,
        sinf(getRunningTime()) * 20.0f,
        cosf(getRunningTime()) * 170.0f
      );
    };
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