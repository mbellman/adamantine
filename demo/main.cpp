#include "Window.h"
#include "opengl/OpenGLVideoController.h"
#include "DefaultScene.h"
#include "LightRoomScene.h"

int main(int argc, char *argv[]) {
  Window window;
  auto* controller = new OpenGLVideoController();

  controller->setScene(new LightRoomScene());

  window.setVideoController(controller);
  window.open("Adamantine", { 100, 100, 1200, 720 });
  window.run();

  return 0;
}