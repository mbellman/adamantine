#include "SDL.h"
#include "subsystem/InputSystem.h"

void InputSystem::handleKeyDown(const SDL_Keycode& code) {
  switch (code) {
    case SDLK_w:
      keyState |= Key::W;
      break;
    case SDLK_a:
      keyState |= Key::A;
      break;
    case SDLK_s:
      keyState |= Key::S;
      break;
    case SDLK_d:
      keyState |= Key::D;
      break;
  }
}

void InputSystem::handleKeyUp(const SDL_Keycode& code) {
  switch (code) {
    case SDLK_w:
      keyState &= ~Key::W;
      break;
    case SDLK_a:
      keyState &= ~Key::A;
      break;
    case SDLK_s:
      keyState &= ~Key::S;
      break;
    case SDLK_d:
      keyState &= ~Key::D;
      break;
  }
}

bool InputSystem::isKeyHeld(Key key) const {
  return keyState & key;
}

void InputSystem::onKeyDown(KeyboardHandler handler) {
  keyboardHandlers.push_back(handler);
}

void InputSystem::onMouseButton(MouseButtonHandler handler) {
  mouseButtonHandlers.push_back(handler);
}

void InputSystem::onMouseMotion(MouseMotionHandler handler) {
  mouseMotionHandlers.push_back(handler);
}

void InputSystem::handleEvent(const SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN:
      handleKeyDown(event.key.keysym.sym);
      fireInputHandlers(keyboardHandlers, event.key);
      break;
    case SDL_KEYUP:
      handleKeyUp(event.key.keysym.sym);
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      fireInputHandlers(mouseButtonHandlers, event.button);
      break;
    case SDL_MOUSEMOTION:
      fireInputHandlers(mouseMotionHandlers, event.motion);
      break;
  }
}