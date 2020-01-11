#pragma once

#include <functional>
#include <vector>

#include "SDL.h"

template<typename T>
using InputHandler = std::function<void(T)>;

typedef InputHandler<SDL_KeyboardEvent> KeyboardHandler;
typedef InputHandler<SDL_MouseMotionEvent> MouseMotionHandler;
typedef InputHandler<SDL_MouseButtonEvent> MouseButtonHandler;

enum Key {
  W = 1 << 0,
  A = 1 << 1,
  S = 1 << 2,
  D = 1 << 3
};

class InputSystem {
public:
  void handleEvent(const SDL_Event& event);
  bool isKeyHeld(Key key) const;
  void onKeyDown(KeyboardHandler handler);
  void onMouseButton(MouseButtonHandler handler);
  void onMouseMotion(MouseMotionHandler handler);

private:
  int keyState = 0;
  std::vector<KeyboardHandler> keyboardHandlers;
  std::vector<MouseMotionHandler> mouseMotionHandlers;
  std::vector<MouseButtonHandler> mouseButtonHandlers;

  template<typename T, typename E>
  void fireInputHandlers(const std::vector<T>& handlers, const E& event) {
    for (auto handler : handlers) {
      handler(event);
    }
  }

  void handleKeyDown(const SDL_Keycode& code);
  void handleKeyUp(const SDL_Keycode& code);
};