#include "Window.h"
#include "OpenGLVideoController.h"
#include "Geometry.h"

int main(int argc, char *argv[]) {
  Window window;

  window.setVideoController(new OpenGLVideoController());
  window.open("Adamantine", { 100, 100, 640, 480 });
  window.poll();

  return 0;
}