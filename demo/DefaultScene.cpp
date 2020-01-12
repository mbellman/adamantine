#include <cmath>

#include "DefaultScene.h"
#include "subsystem/Entities.h"
#include "subsystem/RNG.h"
#include "subsystem/ObjLoader.h"

void DefaultScene::onInit() {
  settings.backgroundColor = {
    0.5f,
    0.0f,
    0.3f
  };

  auto* mesh = new Mesh(100, 100, 10.0f);

  mesh->setPosition({ 0.0f, -20.0f, 0.0f });

  mesh->defineOffsets([=](Vec3f& position, int x, int y) {
    position.y = RNG::random() * 5.0f;
  });

  auto* cube1 = new Cube();

  cube1->setScale(8.0f);
  cube1->setPosition({ -25.0f, 10.0f, 50.0f });
  cube1->setOrientation({ 1.5f, 0.7f, 2.1f });

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

  cube3->onUpdate = [=](float dt) {
    cube3->rotate({
      0.5f * dt,
      0.5f * dt,
      0.5f * dt
    });
  };

  ObjLoader daVinci("./demo/da-vinci.obj");

  auto* model = new Model(daVinci);

  model->setScale(30.0f);
  model->setPosition({ 0.0f, 15.0f, 250.0f });

  model->onUpdate = [=](float dt) {
    model->rotate({ 0.0f, dt * 0.5f, 0.0f });
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

  auto* model2 = new Model(teapot);

  model2->setScale(15.0f);
  model2->setPosition({ 100.0f, 0.0f, 300.0f });

  stage.add(mesh);
  stage.add(cube1);
  stage.add(cube2);
  stage.add(cube3);
  stage.add(model);
  stage.add(tinyCube);
  stage.add(model2);

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