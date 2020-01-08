#include <cmath>
#include <ctime>

#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"
#include "opengl/OpenGLVideoController.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"
#include "subsystem/Math.h"
#include "subsystem/Entities.h"

OpenGLVideoController::OpenGLVideoController() {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

OpenGLVideoController::~OpenGLVideoController() {
  for (auto [ key, pipeline ] : pipelines) {
    delete pipeline;
  }

  pipelines.clear();
}

OpenGLPipeline* OpenGLVideoController::createOpenGLPipeline(const Object* object) {
  OpenGLPipeline* pipeline = new OpenGLPipeline();

  pipeline->createFromObject(object);

  return pipeline;
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

  return (Matrix4::rotate(camera.orientation) * Matrix4::translate(camera.position)).transpose();
}

SDL_Window* OpenGLVideoController::createWindow(const char* title, Region2d<int> region) {
  return SDL_CreateWindow(title, region.x, region.y, region.width, region.height, SDL_WINDOW_OPENGL);
}

void OpenGLVideoController::onDestroy() {
  SDL_GL_DeleteContext(glContext);
}

void OpenGLVideoController::onEntityAdded(Entity* entity) {
  if (entity->isOfType<Object>()) {
    auto* pipeline = createOpenGLPipeline((Object*)entity);

    pipeline->bind();
    geometry.bindVertexInputs();

    pipelines.emplace(entity->id, pipeline);
  }
}

void OpenGLVideoController::onEntityRemoved(Entity* entity) {
  if (entity->isOfType<Object>()) {
    delete pipelines.at(entity->id);

    pipelines.erase(entity->id);
  }
}

void OpenGLVideoController::onInit() {
  glContext = SDL_GL_CreateContext(sdlWindow);
  glewExperimental = true;

  glewInit();

  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  SDL_GL_SetSwapInterval(0);

  lighting.create();
  lighting.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/l-vertex.glsl"));
  lighting.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/l-fragment.glsl"));
  lighting.link();

  geometry.create();
  geometry.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/g-vertex.glsl"));
  geometry.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/g-fragment.glsl"));
  geometry.link();
  geometry.use();

  VertexShaderInput vertexShaderInputs[4] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  geometry.saveVertexInputs<float>(4, vertexShaderInputs);

  scene->onEntityAdded([=](auto* entity) {
    onEntityAdded(entity);
  });

  scene->onEntityRemoved([=](auto* entity) {
    onEntityRemoved(entity);
  });

  scene->onInit();

  gBuffer.initialize(1200, 720);
}

void OpenGLVideoController::onRender() {
  const Vec3f& backgroundColor = scene->getSettings().backgroundColor;

  gBuffer.allowWriting();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(geometry.getUniformLocation("projectionMatrix"), 1, GL_FALSE, createProjectionMatrix(45.0f, 1200.0f / 720.0f, 1.0f, 10000.0f).m);
  glUniformMatrix4fv(geometry.getUniformLocation("viewMatrix"), 1, GL_FALSE, createViewMatrix().m);

  for (auto* object : scene->getStage().getObjects()) {
    glUniformMatrix4fv(geometry.getUniformLocation("modelMatrix"), 1, GL_FALSE, object->getMatrix().m);

    pipelines.at(object->id)->render();
  }

  gBuffer.useDefaultFrameBuffer();
  gBuffer.allowReading();
  gBuffer.attach(GBuffer::Attachment::COLOR);

  glClear(GL_COLOR_BUFFER_BIT);
  glBlitFramebuffer(0, 0, 1200, 720, 0, 0, 1200, 720, GL_COLOR_BUFFER_BIT, GL_LINEAR);

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  glViewport(0, 0, width, height);
}