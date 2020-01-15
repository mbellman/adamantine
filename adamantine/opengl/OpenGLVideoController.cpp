#include <cmath>
#include <ctime>
#include <cstdio>

#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"
#include "opengl/OpenGLVideoController.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"
#include "opengl/FrameBuffer.h"
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

  for (auto [ key, texture ] : textures) {
    delete texture;
  }

  pipelines.clear();
  textures.clear();

  delete lightingPipeline;
  delete gBuffer;
}

void OpenGLVideoController::createGBuffer() {
  if (gBuffer != nullptr) {
    delete gBuffer;
  }

  lighting.use();

  gBuffer = new FrameBuffer(screenSize.width, screenSize.height);

  gBuffer->addColorBuffer(GL_RGB32F, GL_RGB);
  glUniform1i(lighting.getUniformLocation("colorTexture"), 0);

  gBuffer->addColorBuffer(GL_RGBA32F, GL_RGBA);
  glUniform1i(lighting.getUniformLocation("normalDepthTexture"), 1);

  gBuffer->addColorBuffer(GL_RGB32F, GL_RGB);
  glUniform1i(lighting.getUniformLocation("positionTexture"), 2);

  gBuffer->addDepthBuffer();
  gBuffer->initializeColorBuffers();
}

void OpenGLVideoController::createGeometryProgram() {
  geometry.create();
  geometry.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/g-vertex.glsl"));
  geometry.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/g-fragment.glsl"));
  geometry.link();
  geometry.use();

  VertexShaderInput vertexShaderInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  geometry.setVertexInputs<float>(4, vertexShaderInputs);
}

void OpenGLVideoController::createLightingProgram() {
  lighting.create();
  lighting.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/l-vertex.glsl"));
  lighting.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/l-fragment.glsl"));
  lighting.link();
  lighting.use();

  VertexShaderInput inputs[] = {
    { "vertexPosition", 2, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  lighting.setVertexInputs<float>(2, inputs);

  float lightingQuad[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f
  };

  lightingPipeline = new OpenGLPipeline();

  lighting.bindVertexInputs();

  lightingPipeline->pipe(24, lightingQuad);
  lightingPipeline->setTotalVertices(6);
}

OpenGLPipeline* OpenGLVideoController::createOpenGLPipeline(const Object* object) {
  OpenGLPipeline* pipeline = new OpenGLPipeline();

  pipeline->createFromObject(object);

  return pipeline;
}

Matrix4 OpenGLVideoController::createProjectionMatrix(float fov, float near, float far) {
  constexpr float DEG_TO_RAD = M_PI / 180.0f;
  float f = 1.0f / tanf(fov / 2.0f * DEG_TO_RAD);
  float aspectRatio = (float)screenSize.width / (float)screenSize.height;

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

  Vec3f rotation = {
    -camera.orientation.x,
    camera.orientation.y,
    camera.orientation.z
  };

  Vec3f translation = {
    -camera.position.x,
    -camera.position.y,
    camera.position.z
  };

  return (
    Matrix4::rotate(rotation) *
    Matrix4::translate(translation)
  ).transpose();
}

SDL_Window* OpenGLVideoController::createWindow(const char* title, Region2d<int> region) {
  return SDL_CreateWindow(title, region.x, region.y, region.width, region.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
}

void OpenGLVideoController::onDestroy() {
  SDL_GL_DeleteContext(glContext);
}

void OpenGLVideoController::onEntityAdded(Entity* entity) {
  if (entity->isOfType<Object>()) {
    auto* object = (Object*)entity;
    auto* pipeline = createOpenGLPipeline(object);

    pipeline->bind();
    geometry.bindVertexInputs();
    pipelines.emplace(object->id, pipeline);

    if (object->texture != nullptr && textures.find(object->texture->getId()) == textures.end()) {
      auto* texture = new OpenGLTexture(object->texture);

      textures.emplace(object->texture->getId(), texture);
    }
  }
}

void OpenGLVideoController::onEntityRemoved(Entity* entity) {
  if (entity->isOfType<Object>()) {
    auto* object = (Object*)entity;

    delete pipelines.at(object->id);

    pipelines.erase(object->id);
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

  createLightingProgram();
  createGeometryProgram();
  createGBuffer();

  scene->onEntityAdded([=](auto* entity) {
    onEntityAdded(entity);
  });

  scene->onEntityRemoved([=](auto* entity) {
    onEntityRemoved(entity);
  });

  scene->onInit();
}

void OpenGLVideoController::onRender() {
  const Vec3f& backgroundColor = scene->getSettings().backgroundColor;

  renderGeometry();
  renderLighting();

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  screenSize.width = width;
  screenSize.height = height;

  glViewport(0, 0, width, height);
  createGBuffer();
}

void OpenGLVideoController::renderGeometry() {
  gBuffer->startWriting();
  geometry.use();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(geometry.getUniformLocation("projectionMatrix"), 1, GL_FALSE, createProjectionMatrix(45.0f, 1.0f, 10000.0f).m);
  glUniformMatrix4fv(geometry.getUniformLocation("viewMatrix"), 1, GL_FALSE, createViewMatrix().m);

  for (auto* object : scene->getStage().getObjects()) {
    glUniformMatrix4fv(geometry.getUniformLocation("modelMatrix"), 1, GL_FALSE, object->getMatrix().m);
    
    if (object->texture != nullptr) {
      glUniform1i(geometry.getUniformLocation("hasTexture"), 1);
      glUniform1i(geometry.getUniformLocation("modelTexture"), 3);
      glActiveTexture(GL_TEXTURE0 + 3);
      textures.at(object->texture->getId())->use();
    } else {
      glUniform1i(geometry.getUniformLocation("hasTexture"), 0);
    }

    pipelines.at(object->id)->render();
  }
}

void OpenGLVideoController::renderLighting() {
  gBuffer->useDefaultFramebuffer();
  gBuffer->startReading();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT);

  lighting.use();

  const Vec3f cameraPosition = scene->getCamera().position;

  float camera[3] = {
    cameraPosition.x,
    cameraPosition.y,
    cameraPosition.z
  };

  glUniform3fv(lighting.getUniformLocation("cameraPosition"), 1, camera);

  lightingPipeline->render();
}