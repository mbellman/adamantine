#include "PalaceScene.h"
#include "subsystem/Texture.h"
#include "subsystem/Entities.h"
#include "subsystem/Math.h"

void PalaceScene::addLights() {
  auto* light = new Light();

  light->type = Light::LightType::DIRECTIONAL;
  light->color = Vec3f(0.1f, 0.5f, 1.0f);
  light->direction = Vec3f(1.0f, -1.0f, 1.0f);
  light->canCastShadows = true;

  auto* light2 = new Light();

  light2->type = Light::LightType::DIRECTIONAL;
  light2->color = Vec3f(1.0f, 0.5f, 1.0f);
  light2->direction = Vec3f(-1.0f, -1.0f, -1.0f);
  light2->canCastShadows = true;

  stage.add(light);
  stage.add(light2);
}

void PalaceScene::addObjects() {
  auto* skyTexture = new Texture("./demo/clouds-skybox.png");
  auto* skybox = new Skybox(skyTexture, 5000.0f);

  skybox->onUpdate = [=](float dt) {
    skybox->setPosition(camera.position);
  };

  ObjLoader pagodaObj("./demo/pagoda.obj");

  auto* pagoda = new Model(pagodaObj);

  pagoda->setScale(100.0f);
  pagoda->setPosition({ 0.0f, 0.0f, 300.0f });
  pagoda->setColor(Vec3f(0.5f));

  stage.add(skybox);
  stage.add(pagoda);
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