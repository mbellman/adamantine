#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"
#include "OpenGLVideoController.h"
#include "VertexPipeline.h"
#include "ShaderProgram.h"
#include "ShaderLoader.h"
#include "Math.h"
#include "Entities.h"

SDL_Window* OpenGLVideoController::createWindow(const char* title, Region2d<int> region) {
  return SDL_CreateWindow(title, region.x, region.y, region.width, region.height, SDL_WINDOW_OPENGL);
}

Matrix4 OpenGLVideoController::createProjectionMatrix() {
  return {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f
  };
}

Matrix4 OpenGLVideoController::createViewMatrix() {
  return Matrix4::fromMatrix3(scene.getCamera().getRotationMatrix());
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

  Cube* cube = new Cube();

  cube->setPosition({ 0.0f, 0.0f, -50.0f });
  cube->setScale(15.0f);

  scene.addObject(cube);

  shaderProgram.create();
  shaderProgram.attachShader(ShaderLoader::loadVertexShader("./shaders/vertex.glsl"));
  shaderProgram.attachShader(ShaderLoader::loadFragmentShader("./shaders/fragment.glsl"));
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
  glUniformMatrix4fv(shaderProgram.getUniformLocation("projectionMatrix"), 0, GL_FALSE, createProjectionMatrix().m);
  glUniformMatrix4fv(shaderProgram.getUniformLocation("viewMatrix"), 0, GL_FALSE, createViewMatrix().m);

  for (auto* openGLObject : scene.getOpenGLObjects()) {
    const Object* baseObject = openGLObject->baseObject;

    glUniformMatrix4fv(shaderProgram.getUniformLocation("modelMatrix"), 0, GL_FALSE, baseObject->getMatrix().m);

    openGLObject->pipeline->useVAO();
    glDrawArrays(GL_TRIANGLES, 0, baseObject->getPolygons().size() * 3);
  }

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  glViewport(0, 0, width, height);
}