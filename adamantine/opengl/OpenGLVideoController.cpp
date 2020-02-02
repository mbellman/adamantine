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

void OpenGLVideoController::createScreenShaders() {
  auto* dofShader = new ScreenShader("./adamantine/shaders/dof.fragment.glsl");

  dofShader->onCreateFrameBuffer([=](const ShaderProgram& program, auto screen) {
    auto* buffer = new FrameBuffer(screen.width, screen.height);

    buffer->addColorTexture(GL_RGBA32F, GL_RGBA);
    glUniform1i(program.getUniformLocation("screen"), 0);

    buffer->initializeColorTextures();

    return buffer;
  });

  dofShader->onRender([=](const ShaderProgram& program, OpenGLPipeline* glScreenQuad) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1i(program.getUniformLocation("screen"), 0);

    glScreenQuad->render();
  });

  dofShader->createFrameBuffer(screenSize);
  screenShaders.push_back(dofShader);
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

    gBuffer->getGeometryProgram().bindVertexInputs();
    gBuffer->getLightViewProgram().bindVertexInputs();

    glObjects.push_back(glObject);
  } else if (entity->isOfType<Light>() && ((Light*)entity)->canCastShadows) {
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

  gBuffer = new GBuffer();

  gBuffer->createFrameBuffer(screenSize.width, screenSize.height);

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
  gBuffer->startWriting();

  renderGeometry();

  screenShaders[0]->startWriting();
  gBuffer->startReading();

  renderLighting();
  renderShadowCasters();
  renderScreenShaders();

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}

void OpenGLVideoController::onScreenSizeChange(int width, int height) {
  screenSize.width = width;
  screenSize.height = height;

  glViewport(0, 0, width, height);

  gBuffer->createFrameBuffer(width, height);

  for (auto* shader : screenShaders) {
    shader->createFrameBuffer({ 0, 0, width, height });
  }
}

void OpenGLVideoController::renderShadowCasters() {
  auto& lightViewProgram = gBuffer->getLightViewProgram();
  auto& shadowCasterProgram = gBuffer->getShadowCasterProgram();

  for (auto* glShadowCaster : glShadowCasters) {
    // Shadowcaster light space pass
    lightViewProgram.use();

    gBuffer->startWriting();
    gBuffer->clearLightViewBuffer();

    auto* light = glShadowCaster->getLight();
    const Vec3f& cameraPosition = scene->getCamera().position;
    Vec3f lightPosition = (cameraPosition - light->direction.unit() * 100.0f);

    Matrix4 projection = Matrix4::orthographic(200.0f, -200.0f, -200.0f, 200.0f, -1000.0f, 1000.0f);
    Matrix4 view = Matrix4::lookAt(lightPosition * Vec3f(-1.0f, -1.0f, 1.0f), light->direction * Vec3f(-1.0f, -1.0f, 1.0f));
    Matrix4 lightMatrix = (projection * view).transpose();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(lightViewProgram.getUniformLocation("lightMatrix"), 1, GL_FALSE, lightMatrix.m);

    for (auto* glObject : glObjects) {
      const float* modelMatrix = glObject->getSourceObject()->getMatrix().m;

      glUniformMatrix4fv(lightViewProgram.getUniformLocation("modelMatrix"), 1, GL_FALSE, modelMatrix);

      glObject->render();
    }

    // Shadowcaster camera space lighting pass
    screenShaders[0]->startWriting();
    gBuffer->startReading();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);

    shadowCasterProgram.use();

    glUniform1i(shadowCasterProgram.getUniformLocation("colorTexture"), 0);
    glUniform1i(shadowCasterProgram.getUniformLocation("normalDepthTexture"), 1);
    glUniform1i(shadowCasterProgram.getUniformLocation("positionTexture"), 2);
    glUniform1i(shadowCasterProgram.getUniformLocation("lightMap"), 3);
    glUniformMatrix4fv(shadowCasterProgram.getUniformLocation("lightMatrix"), 1, GL_FALSE, lightMatrix.m);
    glUniform3fv(shadowCasterProgram.getUniformLocation("cameraPosition"), 1, scene->getCamera().position.float3());

    glUniform3fv(shadowCasterProgram.getUniformLocation("light.position"), 1, light->position.float3());
    glUniform3fv(shadowCasterProgram.getUniformLocation("light.direction"), 1, light->direction.float3());
    glUniform3fv(shadowCasterProgram.getUniformLocation("light.color"), 1, light->color.float3());
    glUniform1f(shadowCasterProgram.getUniformLocation("light.radius"), light->radius);
    glUniform1i(shadowCasterProgram.getUniformLocation("light.type"), light->type);

    gBuffer->renderScreenQuad();
  }

  glDisable(GL_BLEND);
}

void OpenGLVideoController::renderGeometry() {
  auto& geometryProgram = gBuffer->getGeometryProgram();

  geometryProgram.use();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Matrix4 projectionMatrix = Matrix4::projection(screenSize, 45.0f, 1.0f, 10000.0f).transpose();
  Matrix4 viewMatrix = createViewMatrix();

  glUniformMatrix4fv(geometryProgram.getUniformLocation("projectionMatrix"), 1, GL_FALSE, projectionMatrix.m);
  glUniformMatrix4fv(geometryProgram.getUniformLocation("viewMatrix"), 1, GL_FALSE, viewMatrix.m);
  glUniform1i(geometryProgram.getUniformLocation("modelTexture"), 4);
  glUniform1i(geometryProgram.getUniformLocation("normalMap"), 5);

  for (auto* glObject : glObjects) {
    const float* modelMatrix = glObject->getSourceObject()->getMatrix().m;

    glUniformMatrix4fv(geometryProgram.getUniformLocation("modelMatrix"), 1, GL_FALSE, modelMatrix);
    glUniform1i(geometryProgram.getUniformLocation("hasTexture"), glObject->hasTexture());
    glUniform1i(geometryProgram.getUniformLocation("hasNormalMap"), glObject->hasNormalMap());

    glObject->render();
  }
}

void OpenGLVideoController::renderLighting() {
  auto& lightingProgram = gBuffer->getLightingProgram();

  lightingProgram.use();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT);

  auto& lights = scene->getStage().getLights();
  int totalLights = lights.size() - scene->getStage().getTotalShadowCasters();

  glUniform1i(lightingProgram.getUniformLocation("colorTexture"), 0);
  glUniform1i(lightingProgram.getUniformLocation("normalDepthTexture"), 1);
  glUniform1i(lightingProgram.getUniformLocation("positionTexture"), 2);
  glUniform3fv(lightingProgram.getUniformLocation("cameraPosition"), 1, scene->getCamera().position.float3());
  glUniform1i(lightingProgram.getUniformLocation("totalLights"), totalLights);

  int index = 0;

  for (auto* light : lights) {
    if (!light->canCastShadows) {
      std::string idx = std::to_string(index++);

      glUniform3fv(lightingProgram.getUniformLocation("lights[" + idx + "].position"), 1, light->position.float3());
      glUniform3fv(lightingProgram.getUniformLocation("lights[" + idx + "].direction"), 1, light->direction.float3());
      glUniform3fv(lightingProgram.getUniformLocation("lights[" + idx + "].color"), 1, light->color.float3());
      glUniform1f(lightingProgram.getUniformLocation("lights[" + idx + "].radius"), light->radius);
      glUniform1i(lightingProgram.getUniformLocation("lights[" + idx + "].type"), light->type);
    }
  }

  gBuffer->renderScreenQuad();
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