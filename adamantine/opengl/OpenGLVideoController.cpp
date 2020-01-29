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

  for (auto* glShadowCaster : glShadowCasters) {
    delete glShadowCaster;
  }

  screenShaders.clear();
  glObjects.clear();
  glShadowCasters.clear();

  OpenGLObject::freeTextureCache();
}

void OpenGLVideoController::createDepthProgram() {
  depth.create();
  depth.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/depth.vertex.glsl"));
  depth.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/depth.fragment.glsl"));
  depth.link();
  depth.use();

  VertexShaderInput vertexInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  depth.setVertexInputs<float>(5, vertexInputs);
}

void OpenGLVideoController::createGeometryProgram() {
  geometry.create();
  geometry.attachShader(ShaderLoader::loadVertexShader("./adamantine/shaders/geometry.vertex.glsl"));
  geometry.attachShader(ShaderLoader::loadFragmentShader("./adamantine/shaders/geometry.fragment.glsl"));
  geometry.link();
  geometry.use();

  VertexShaderInput vertexInputs[] = {
    { "vertexPosition", 3, GL_FLOAT },
    { "vertexNormal", 3, GL_FLOAT},
    { "vertexTangent", 3, GL_FLOAT },
    { "vertexColor", 3, GL_FLOAT },
    { "vertexUv", 2, GL_FLOAT }
  };

  geometry.setVertexInputs<float>(5, vertexInputs);
}

void OpenGLVideoController::createScreenShaders() {
  auto* depthShader = new ScreenShader("./adamantine/shaders/z.fragment.glsl");

  depthShader->onCreateFrameBuffer([=](const ShaderProgram& program, auto screen) {
    auto* buffer = new FrameBuffer(screen.width, screen.height);

    buffer->addColorBuffer(GL_RGB32F, GL_RGB);
    glUniform1i(program.getUniformLocation("depthTexture"), 0);

    buffer->initializeColorBuffers();

    return buffer;
  });

  depthShader->onRender([=](const ShaderProgram& program, OpenGLPipeline* glScreenQuad) {
    glClear(GL_COLOR_BUFFER_BIT);

    glScreenQuad->render();
  });

  depthShader->createFrameBuffer(screenSize);

  screenShaders.push_back(depthShader);

  // auto* lightingShader = new ScreenShader("./adamantine/shaders/lighting.glsl");
  // auto* dofShader = new ScreenShader("./adamantine/shaders/dof.glsl");

  // lightingShader->onCreateFrameBuffer([=](const ShaderProgram& program, auto screen) {
  //   auto* buffer = new FrameBuffer(screen.width, screen.height);

  //   buffer->addColorBuffer(GL_RGB32F, GL_RGB);
  //   glUniform1i(program.getUniformLocation("colorTexture"), 0);

  //   buffer->addColorBuffer(GL_RGBA32F, GL_RGBA);
  //   glUniform1i(program.getUniformLocation("normalDepthTexture"), 1);

  //   buffer->addColorBuffer(GL_RGB32F, GL_RGB);
  //   glUniform1i(program.getUniformLocation("positionTexture"), 2);

  //   buffer->addDepthBuffer();
  //   buffer->initializeColorBuffers();

  //   return buffer;
  // });

  // lightingShader->onRender([=](const ShaderProgram& program, OpenGLPipeline* glScreenQuad) {
  //   glDisable(GL_DEPTH_TEST);
  //   glDisable(GL_CULL_FACE);
  //   glClear(GL_COLOR_BUFFER_BIT);

  //   auto& lights = scene->getStage().getLights();
  //   int totalLights = lights.size();

  //   glUniform3fv(program.getUniformLocation("cameraPosition"), 1, scene->getCamera().position.float3());
  //   glUniform1i(program.getUniformLocation("totalLights"), totalLights);

  //   for (int i = 0; i < totalLights; i++) {
  //     auto* light = lights[i];
  //     std::string idx = std::to_string(i);

  //     glUniform3fv(program.getUniformLocation("lights[" + idx + "].position"), 1, light->position.float3());
  //     glUniform3fv(program.getUniformLocation("lights[" + idx + "].direction"), 1, light->direction.float3());
  //     glUniform3fv(program.getUniformLocation("lights[" + idx + "].color"), 1, light->color.float3());
  //     glUniform1f(program.getUniformLocation("lights[" + idx + "].radius"), light->radius);
  //     glUniform1i(program.getUniformLocation("lights[" + idx + "].type"), light->type);
  //   }

  //   glScreenQuad->render();
  // });

  // dofShader->onCreateFrameBuffer([=](const ShaderProgram& program, auto screen) {
  //   auto* buffer = new FrameBuffer(screen.width, screen.height);

  //   buffer->addColorBuffer(GL_RGBA32F, GL_RGBA);
  //   glUniform1i(program.getUniformLocation("screen"), 0);

  //   buffer->initializeColorBuffers();

  //   return buffer;
  // });

  // dofShader->onRender([=](const ShaderProgram& program, OpenGLPipeline* glScreenQuad) {
  //   glClear(GL_COLOR_BUFFER_BIT);
  //   glUniform1i(program.getUniformLocation("screen"), 0);

  //   glScreenQuad->render();
  // });

  // lightingShader->createFrameBuffer(screenSize);
  // dofShader->createFrameBuffer(screenSize);

  // screenShaders.push_back(lightingShader);
  // screenShaders.push_back(dofShader);
}

Matrix4 OpenGLVideoController::createViewMatrix() {
  const Camera& camera = scene->getCamera();
  Vec3f rotation = camera.orientation * Vec3f(-1.0f, 1.0f, 1.0f);
  Vec3f translation = camera.position * Vec3f(-1.0f, -1.0f, 1.0f);

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
    depth.bindVertexInputs();

    glObjects.push_back(glObject);
  } else if (entity->isOfType<Light>() && ((Light*)entity)->type == Light::LightType::DIRECTIONAL) {
    auto* glShadowCaster = new OpenGLShadowCaster((Light*)entity);

    glShadowCasters.push_back(glShadowCaster);
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

  createDepthProgram();
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
  // screenShaders[0]->startWriting();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  renderShadowCasters();
  // renderGeometry();
  // renderScreenShaders();

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

void OpenGLVideoController::renderShadowCasters() {
  depth.use();

  // glViewport(0, 0, 1024, 1024);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  for (auto* glShadowCaster : glShadowCasters) {
    // glShadowCaster->startCasting();

    Matrix4 lightMatrix = glShadowCaster->getLightMatrix().transpose();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(depth.getUniformLocation("lightMatrix"), 1, GL_FALSE, lightMatrix.m);

    for (auto* glObject : glObjects) {
      const float* modelMatrix = glObject->getSourceObject()->getMatrix().m;

      glUniformMatrix4fv(depth.getUniformLocation("modelMatrix"), 1, GL_FALSE, modelMatrix);

      glObject->render();
    }
  }

  // glViewport(0, 0, screenSize.width, screenSize.height);
}

void OpenGLVideoController::renderGeometry() {
  geometry.use();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Matrix4 projectionMatrix = Matrix4::projection(screenSize, 45.0f, 1.0f, 10000.0f).transpose();
  Matrix4 viewMatrix = createViewMatrix();

  glUniformMatrix4fv(geometry.getUniformLocation("projectionMatrix"), 1, GL_FALSE, projectionMatrix.m);
  glUniformMatrix4fv(geometry.getUniformLocation("viewMatrix"), 1, GL_FALSE, viewMatrix.m);
  glUniform1i(geometry.getUniformLocation("modelTexture"), 3);
  glUniform1i(geometry.getUniformLocation("normalMap"), 4);

  for (auto* glObject : glObjects) {
    const float* modelMatrix = glObject->getSourceObject()->getMatrix().m;

    glUniformMatrix4fv(geometry.getUniformLocation("modelMatrix"), 1, GL_FALSE, modelMatrix);
    glUniform1i(geometry.getUniformLocation("hasTexture"), glObject->hasTexture());
    glUniform1i(geometry.getUniformLocation("hasNormalMap"), glObject->hasNormalMap());

    glObject->render();
  }
}

void OpenGLVideoController::renderScreenShaders() {
  for (int i = 0; i < screenShaders.size(); i++) {
    bool isFinalShader = i == screenShaders.size() - 1;

    if (isFinalShader) {
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    } else {
      screenShaders[i + 1]->startWriting();
    }

    screenShaders[i]->render();
  }
}