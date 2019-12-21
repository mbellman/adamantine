#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>

#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"
#include "opengl/OpenGLVideoController.h"
#include "opengl/VertexPipeline.h"
#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"
#include "subsystem/Math.h"
#include "subsystem/Entities.h"

SDL_Window* OpenGLVideoController::createWindow(const char* title, Region2d<int> region) {
  return SDL_CreateWindow(title, region.x, region.y, region.width, region.height, SDL_WINDOW_OPENGL);
}

Matrix4 OpenGLVideoController::createProjectionMatrix(float fov, float aspectRatio, float near, float far) {
  constexpr float DEG_TO_RAD = M_PI / 180.0f;
  float f = 1.0f / tanf(fov / 2.0f * DEG_TO_RAD);

  Matrix4 projection = {
    f / aspectRatio, 0.0f, 0.0f, 0.0f,
    0.0f, f, 0.0f, 0.0f,
    0.0f, 0.0f, (far + near) / (near - far), (2 * far * near) / (near - far),
    0.0f, 0.0f, -1.0f, 0.0f
  };

  return projection.transpose();
}

Matrix4 OpenGLVideoController::createViewMatrix() {
  const Camera& camera = scene->getCamera();

  Matrix4 translation = Matrix4::translate({
    camera.position.x,
    camera.position.y,
    camera.position.z * -1.0f
  });

  return (Matrix4::rotate(camera.orientation) * translation).transpose();
}

void OpenGLVideoController::onDestroy() {
  SDL_GL_DeleteContext(glContext);
}

void OpenGLVideoController::onInit() {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  glContext = SDL_GL_CreateContext(sdlWindow);

  SDL_GL_SetSwapInterval(0);

  glewExperimental = true;
  glewInit();

  scene->onInit();

  shaderProgram.create();
  shaderProgram.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/vertex.glsl"));
  shaderProgram.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/fragment.glsl"));
  shaderProgram.activate();
  shaderProgram.setFragmentShaderOutput("color");

  VertexShaderInput<float> vertexPositionInput = { "vertexPosition", 3, GL_FLOAT, 6, 0 };
  VertexShaderInput<float> vertexColorInput = { "vertexColor", 3, GL_FLOAT, 6, 3 };

  shaderProgram.setVertexShaderInput(vertexPositionInput);
  shaderProgram.setVertexShaderInput(vertexColorInput);
}

void OpenGLVideoController::onRender() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform1f(shaderProgram.getUniformLocation("time"), SDL_GetTicks() / 500.0f);
  glUniformMatrix4fv(shaderProgram.getUniformLocation("projectionMatrix"), 1, GL_FALSE, createProjectionMatrix(45.0f, 1200.0f / 720.0f, 1.0f, 1000.0f).m);
  glUniformMatrix4fv(shaderProgram.getUniformLocation("viewMatrix"), 1, GL_FALSE, createViewMatrix().m);

  for (auto* object : scene->getEntityContainer().getObjects()) {
    glUniformMatrix4fv(shaderProgram.getUniformLocation("modelMatrix"), 1, GL_FALSE, object->getMatrix().m);

    ((OpenGLObject*)object)->pipeline->useVAO();
    glDrawArrays(GL_TRIANGLES, 0, object->getPolygons().size() * 3);
  }

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  glViewport(0, 0, width, height);
}