#include "Window.h"
#include "opengl/OpenGLVideoController.h"

int main(int argc, char *argv[]) {
  Window window;

  window.setVideoController(new OpenGLVideoController());
  window.open("Adamantine", { 100, 100, 1200, 720 });
  window.poll();

  return 0;
}