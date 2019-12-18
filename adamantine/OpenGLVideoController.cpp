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

void OpenGLVideoController::createVertexPipelineFromObject(Object* object) {
  const std::vector<Polygon*>& polygons = object->getPolygons();
}

SDL_Window* OpenGLVideoController::createWindow(const char* title, Region2d<int> region) {
  return SDL_CreateWindow(title, region.x, region.y, region.width, region.height, SDL_WINDOW_OPENGL);
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

  shaderProgram.create();
  shaderProgram.attachShader(ShaderLoader::loadVertexShader("./shaders/vertex.glsl"));
  shaderProgram.attachShader(ShaderLoader::loadFragmentShader("./shaders/fragment.glsl"));
  shaderProgram.activate();
  shaderProgram.setFragmentShaderOutput("color");

  VertexShaderInput<float> vertexPositionInput = { "position", 2, GL_FLOAT, 5, 0 };
  VertexShaderInput<float> vertexColorInput = { "vertexColor", 3, GL_FLOAT, 5, 2 };

  shaderProgram.setVertexShaderInput(vertexPositionInput);
  shaderProgram.setVertexShaderInput(vertexColorInput);

  Cube* cube = new Cube();

  cube->position = { 0.0f, 0.0f, -10.0f };

  scene.addObject(cube);
  createVertexPipelineFromObject(cube);

  cameraMatrix = shaderProgram.createUniformInput("cameraMatrix");
  time = shaderProgram.createUniformInput("time");
}

void OpenGLVideoController::onRender() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  Matrix3 rotationMatrix = scene.getCamera().getRotationMatrix();

  glUniform1f(time, SDL_GetTicks() / 500.0f);

  for (auto* object : scene.getObjects()) {
    const Vec3f& position = object->position;
    const Vec3f& orientation = object->orientation;

    glLoadIdentity();
    glScalef(object->scale, object->scale, object->scale);
    glRotatef(orientation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(orientation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(orientation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(position.x, position.y, position.z);

    // glUniformMatrix3fv(cameraMatrix, 0, GL_FALSE, rotationMatrix);
  }

  for (auto* pipeline : pipelines) {
    pipeline->useArray();
    glDrawArrays(GL_TRIANGLES, 0, 10000);
  }

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  glViewport(0, 0, width, height);
}