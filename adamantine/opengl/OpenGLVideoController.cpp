#include <cmath>
#include <ctime>
#include <cstdio>

#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"
#include "opengl/OpenGLVideoController.h"
#include "opengl/OpenGLObject.h"
#include "opengl/OpenGLPipeline.h"
#include "opengl/ShaderProgram.h"
#include "opengl/ShaderLoader.h"
#include "opengl/ScreenShader.h"
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
  for (auto* glObject : glObjects) {
    delete glObject;
  }

  for (auto* shader : screenShaders) {
    delete shader;
  }

  screenShaders.clear();
  glObjects.clear();

  OpenGLObject::freeTextureCache();
}

void OpenGLVideoController::createGeometryProgram() {
  geometry.create();
  geometry.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/geometry.vertex.glsl"));
  geometry.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/geometry.fragment.glsl"));
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

void OpenGLVideoController::createScreenShaders() {
  auto* lightingShader = new ScreenShader("./adamantine/shaders/lighting.glsl");
  auto* dofShader = new ScreenShader("./adamantine/shaders/dof.glsl");

  lightingShader->onCreateFrameBuffer([=](auto screen, const ShaderProgram& program) {
    auto* buffer = new FrameBuffer(screen.width, screen.height);

    buffer->addColorBuffer(GL_RGB32F, GL_RGB);
    glUniform1i(program.getUniformLocation("colorTexture"), 0);

    buffer->addColorBuffer(GL_RGBA32F, GL_RGBA);
    glUniform1i(program.getUniformLocation("normalDepthTexture"), 1);

    buffer->addColorBuffer(GL_RGB32F, GL_RGB);
    glUniform1i(program.getUniformLocation("positionTexture"), 2);

    buffer->addDepthBuffer();
    buffer->initializeColorBuffers();

    return buffer;
  });

  lightingShader->onRender([=](const ShaderProgram& program) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    const Vec3f& cameraPosition = scene->getCamera().position;

    float camera[3] = {
      cameraPosition.x,
      cameraPosition.y,
      cameraPosition.z
    };

    glUniform3fv(program.getUniformLocation("cameraPosition"), 1, camera);
  });

  dofShader->onCreateFrameBuffer([=](auto screen, const ShaderProgram& program) {
    auto* buffer = new FrameBuffer(screen.width, screen.height);

    buffer->addColorBuffer(GL_RGBA32F, GL_RGBA);
    glUniform1i(program.getUniformLocation("screen"), 0);

    buffer->initializeColorBuffers();

    return buffer;
  });

  dofShader->onRender([=](const ShaderProgram& program) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1i(program.getUniformLocation("screen"), 0);
  });

  lightingShader->createFrameBuffer(screenSize);
  dofShader->createFrameBuffer(screenSize);

  screenShaders.push_back(lightingShader);
  screenShaders.push_back(dofShader);
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
    auto* glObject = new OpenGLObject((Object*)entity);

    glObject->bind();
    geometry.bindVertexInputs();
    glObjects.push_back(glObject);
  }
}

void OpenGLVideoController::onEntityRemoved(Entity* entity) {
  if (entity->isOfType<Object>()) {
    const auto* object = (Object*)entity;
    int index = 0;

    for (auto* glObject : glObjects) {
      if (glObject->getSourceObject() == object) {
        glObjects.erase(glObjects.begin() + index);

        break;
      }

      index++;
    }
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

  createGeometryProgram();
  createScreenShaders();

  scene->onEntityAdded([=](auto* entity) {
    onEntityAdded(entity);
  });

  scene->onEntityRemoved([=](auto* entity) {
    onEntityRemoved(entity);
  });

  scene->onInit();
}

void OpenGLVideoController::onRender() {
  screenShaders[0]->startWriting();
  renderGeometry();

  for (int i = 0; i < screenShaders.size(); i++) {
    if (i == screenShaders.size() - 1) {
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    } else {
      screenShaders[i + 1]->startWriting();
    }

    screenShaders[i]->render();
  }

  SDL_GL_SwapWindow(sdlWindow);

  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  screenSize.width = width;
  screenSize.height = height;

  glViewport(0, 0, width, height);

  for (auto* shader : screenShaders) {
    shader->createFrameBuffer({ 0, 0, width, height });
  }
}

void OpenGLVideoController::renderGeometry() {
  geometry.use();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(geometry.getUniformLocation("projectionMatrix"), 1, GL_FALSE, createProjectionMatrix(45.0f, 1.0f, 10000.0f).m);
  glUniformMatrix4fv(geometry.getUniformLocation("viewMatrix"), 1, GL_FALSE, createViewMatrix().m);
  glUniform1i(geometry.getUniformLocation("modelTexture"), 3);

  for (auto* glObject : glObjects) {
    const float* modelMatrix = glObject->getSourceObject()->getMatrix().m;

    glUniformMatrix4fv(geometry.getUniformLocation("modelMatrix"), 1, GL_FALSE, modelMatrix);
    glUniform1i(geometry.getUniformLocation("hasTexture"), glObject->hasTexture());

    glObject->render();
  }
}