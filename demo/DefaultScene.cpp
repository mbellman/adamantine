#include <cmath>

#include "DefaultScene.h"
#include "subsystem/Entities.h"
#include "subsystem/RNG.h"
#include "subsystem/ObjLoader.h"
#include "subsystem/Texture.h"

void DefaultScene::addLights() {
  auto* directionalLight = new Light();
  auto* directionalLight2 = new Light();
  auto* directionalLight3 = new Light();

  directionalLight->type = Light::LightType::DIRECTIONAL;
  directionalLight->color = Vec3f(1.0f, 0.0f, 0.0f);
  directionalLight->direction = Vec3f(-1.0f, -0.4f, 1.5f);
  directionalLight->canCastShadows = true;

  directionalLight2->type = Light::LightType::DIRECTIONAL;
  directionalLight2->color = Vec3f(0.5f, 0.25f, 0.1f);
  directionalLight2->direction = Vec3f(0.0f, -1.0f, 1.0f);
  directionalLight2->canCastShadows = true;

  directionalLight3->type = Light::LightType::DIRECTIONAL;
  directionalLight3->color = Vec3f(0.1f, 0.25f, 0.5f);
  directionalLight3->direction = Vec3f(1.0f, -1.0f, 1.0f);
  directionalLight3->canCastShadows = true;

  stage.add(directionalLight);
  stage.add(directionalLight2);
  stage.add(directionalLight3);

  for (int i = 0; i < 25; i++) {
    auto* light = new Light();
    float r = i * 10.0f;

    Vec3f position = {
      sinf((float)i) * r,
      20.0f,
      cosf((float)i) * r
    };

    light->position = position;

    light->onUpdate = [=](float dt) {
      light->position = {
        position.x + sinf(getRunningTime()) * 100.0f,
        position.y,
        position.z + cosf(getRunningTime()) * 100.0f
      };
    };

    light->color = {
      RNG::random(),
      RNG::random(),
      RNG::random()
    };

    light->radius = RNG::random(50.0f, 250.0f);

    stage.add(light);
  }
}

void DefaultScene::addObjects() {
  auto* mesh = new Mesh();
  auto* sandNormalMap = new Texture("./demo/sand-normal-map.png");
  auto* catTexture = new Texture("./demo/cat.png");

  assets.addTexture(catTexture);
  assets.addTexture(sandNormalMap);

  mesh->create(4, 4, 500.0f);
  mesh->setPosition({ 0.0f, -20.0f, 0.0f });
  mesh->setColor({ 0.5f, 0.3f, 0.8f });
  mesh->normalMap = sandNormalMap;

  auto* cube1 = new Cube();

  cube1->setScale(8.0f);
  cube1->setPosition({ -25.0f, 10.0f, 50.0f });
  cube1->setOrientation({ 1.5f, 0.7f, 2.1f });
  cube1->texture = catTexture;

  cube1->onUpdate = [=](float dt) {
    cube1->rotate({
      0.5f * dt,
      0.5f * dt,
      0.5f * dt
    });
  };

  auto* cube2 = new Cube();

  cube2->setScale(4.0f);
  cube2->setPosition({ 0.0f, 7.0f, 50.0f });
  cube2->setOrientation({ 0.3f, 1.1f, 0.8f });
  cube2->texture = catTexture;

  cube2->onUpdate = [=](float dt) {
    cube2->rotate({
      0.5f * dt,
      0.5f * dt,
      0.5f * dt
    });
  };

  auto* cube3 = new Cube();

  cube3->setScale(2.0f);
  cube3->setPosition({ 15.0f, 4.0f, 50.0f });
  cube3->setOrientation({ 0.9f, 2.5f, 3.1f });
  cube3->texture = catTexture;

  cube3->onUpdate = [=](float dt) {
    cube3->rotate({
      0.5f * dt,
      0.5f * dt,
      0.5f * dt
    });
  };

  ObjLoader daVinci("./demo/da-vinci.obj");

  auto* model = new Model();

  model->from(daVinci);
  model->setScale(30.0f);
  model->setPosition({ 0.0f, 15.0f, 250.0f });
  model->setColor(Vec3f(0.75f));

  model->onUpdate = [=](float dt) {
    model->rotate({ 0.0f, dt * 0.25f, 0.0f });
  };

  auto* tinyCube = new Cube();

  tinyCube->setScale(3.0f);
  tinyCube->setPosition({ 0.0f, 15.0f, 250.0f });

  tinyCube->onUpdate = [=](float dt) {
    tinyCube->setPosition({
      0.0f + 40.0f * sinf(getRunningTime()),
      15.0f,
      250.0f + 40.0f * cosf(getRunningTime())
    });

    tinyCube->rotate({ 0.0f, -dt, 0.0f });
  };

  ObjLoader teapot("./demo/teapot.obj");

  auto* model2 = new Model();

  model2->from(teapot);
  model2->setScale(15.0f);
  model2->setPosition({ 100.0f, 0.0f, 300.0f });
  model2->setColor({ 0.75f, 0.5f, 0.3f });

  model2->onUpdate = [=](float dt) {
    model2->rotate({
      0.0f,
      0.5f * dt,
      0.0f
    });
  };

  ObjLoader ball("./demo/ball.obj");

  auto* model3 = new Model();

  model3->from(ball);
  model3->setScale(10.0f);
  model3->setPosition({ 80.0f, 10.0f, 200.0f });
  model3->setColor({ 0.6f, 0.1f, 0.3f });

  auto* lightBall = new Model();

  lightBall->from(ball);
  lightBall->setScale(5.0f);
  lightBall->setPosition({ 100.0f, 10.0f, 225.0f });
  lightBall->setColor(Vec3f(0.5f, 0.0f, 1.0f));
  lightBall->isEmissive = true;

  auto* ballLight = new Light();

  ballLight->color = Vec3f(0.5f, 0.0f, 1.0f) * 3.0f;
  ballLight->radius = 1000.0f;
  ballLight->position = lightBall->position;
  ballLight->canCastShadows = true;

  ballLight->onUpdate = [=](float dt) {
    ballLight->position = Vec3f(50.0f, 45.0f, 240.0f) + Vec3f(
      50.0f * sinf(getRunningTime()),
      0.0f,
      50.0f * cosf(getRunningTime())
    );

    lightBall->setPosition(ballLight->position);
  };

  stage.add<Light>([=](Light* light) {
    Vec3f color = Vec3f(1.5f, 0.15f, 0.0f);

    light->type = Light::LightType::POINT;
    light->position = Vec3f(0.0f, 15.0f, 250.0f);
    light->radius = 2000.0f;
    light->canCastShadows = true;

    light->onUpdate = [=](float dt) {
      light->position = Vec3f(0.0f, 15.0f, 250.0f) + Vec3f(0.0f, 10.0f, 0.0f) * sinf(getRunningTime());
      light->color = color * 2.0f * (1.0f + sinf(getRunningTime()));
    };
  });

  stage.add<Model>([=](Model* model) {
    model->from(ball);
    model->setScale(3.0f);
    model->setColor(Vec3f(1.5f, 1.0f, 0.0f));
    model->isEmissive = true;
    
    model->onUpdate = [=](float dt) {
      model->setPosition(Vec3f(0.0f, 15.0f, 250.0f) + Vec3f(0.0f, 10.0f, 0.0f) * sinf(getRunningTime()));
    };
  });

  stage.add(mesh);
  stage.add(cube1);
  stage.add(cube2);
  stage.add(cube3);
  stage.add(model);
  stage.add(tinyCube);
  stage.add(model2);
  stage.add(model3);
  stage.add(lightBall);
  stage.add(ballLight);
}

void DefaultScene::onInit() {
  addLights();
  addObjects();

  auto* skyboxTexture = new Texture("./demo/sunset-skybox.png");
  auto* skybox = new Skybox();

  skybox->from(skyboxTexture);
  skybox->setScale(5000.0f);
  skybox->setColor(Vec3f(1.0f));

  stage.add(skybox);
  assets.addTexture(skyboxTexture);

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