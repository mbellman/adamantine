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
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
}

OpenGLVideoController::~OpenGLVideoController() {
  if (glSkybox != nullptr) {
    delete glSkybox;
  }

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

OpenGLObject* OpenGLVideoController::createOpenGLObject(Object* object) {
  auto* glObject = new OpenGLObject(object);

  glObject->bind();

  gBuffer->getShaderProgram(GBuffer::Shader::GEOMETRY).bindVertexInputs();
  gBuffer->getShaderProgram(GBuffer::Shader::LIGHT_VIEW).bindVertexInputs();

  return glObject;
}

void OpenGLVideoController::createScreenShaders() {
  auto* dofShader = new ScreenShader("./adamantine/shaders/dof.fragment.glsl");

  dofShader->onCreateFrameBuffer([=](const ShaderProgram& program, auto screen) {
    auto* buffer = new FrameBuffer(screen.width, screen.height);

    buffer->addColorTexture(GL_RGBA32F, GL_RGBA, GL_CLAMP_TO_EDGE);   // (0) Color/depth
    buffer->bindColorTextures();

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
  if (entity->isOfType<Skybox>()) {
    if (glSkybox != nullptr) {
      delete glSkybox;
    }

    glSkybox = createOpenGLObject((Skybox*)entity);
  } else if (entity->isOfType<Object>()) {
    auto* glObject = createOpenGLObject((Object*)entity);

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

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glCullFace(GL_BACK);

  SDL_GL_SetSwapInterval(0);

  gBuffer = new GBuffer();

  gBuffer->createFrameBuffer(screenSize.width, screenSize.height);

  createScreenShaders();

  gBuffer->getFrameBuffer()->shareDepthStencilBuffer(screenShaders[0]->getFrameBuffer());

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
  gBuffer->writeToAllBuffers();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  renderGeometry();

  screenShaders[0]->startWriting();
  gBuffer->startReading();

  renderNonIlluminatedSurfaces();
  renderIlluminatedSurfaces();
  renderShadowCasters();
  renderScreenShaders();

  glStencilMask(0xFF);

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

  gBuffer->getFrameBuffer()->shareDepthStencilBuffer(screenShaders[0]->getFrameBuffer());
}

void OpenGLVideoController::renderGeometry() {
  auto& geometryProgram = gBuffer->getShaderProgram(GBuffer::Shader::GEOMETRY);

  geometryProgram.use();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);

  Matrix4 projectionMatrix = Matrix4::projection(screenSize, 45.0f, 1.0f, 10000.0f).transpose();
  Matrix4 viewMatrix = createViewMatrix();

  geometryProgram.setMatrix4("projectionMatrix", projectionMatrix);
  geometryProgram.setMatrix4("viewMatrix", viewMatrix);
  geometryProgram.setInt("modelTexture", 6);
  geometryProgram.setInt("normalMap", 7);

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

  for (auto* glObject : glObjects) {
    geometryProgram.setMatrix4("modelMatrix", glObject->getSourceObject()->getMatrix());
    geometryProgram.setBool("hasTexture", glObject->hasTexture());
    geometryProgram.setBool("hasNormalMap", glObject->hasNormalMap());

    glObject->render();
  }

  glDisable(GL_STENCIL_TEST);
  glStencilMask(0x00);

  if (glSkybox != nullptr) {
    geometryProgram.setMatrix4("modelMatrix", glSkybox->getSourceObject()->getMatrix());
    geometryProgram.setBool("hasTexture", true);
    geometryProgram.setBool("hasNormalMap", false);

    glSkybox->render();
  }
}

void OpenGLVideoController::renderIlluminatedSurfaces() {
  auto& illuminationProgram = gBuffer->getShaderProgram(GBuffer::Shader::ILLUMINATION);

  illuminationProgram.use();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_EQUAL, 1, 0xFF);

  auto& lights = scene->getStage().getLights();
  int totalLights = lights.size() - scene->getStage().getTotalShadowCasters();

  illuminationProgram.setInt("colorTexture", 0);
  illuminationProgram.setInt("normalDepthTexture", 1);
  illuminationProgram.setInt("positionTexture", 2);
  illuminationProgram.setVec3f("cameraPosition", scene->getCamera().position);
  illuminationProgram.setInt("totalLights", totalLights);

  int index = 0;

  for (auto* light : lights) {
    if (!light->canCastShadows) {
      std::string idx = std::to_string(index++);

      illuminationProgram.setVec3f("lights[" + idx + "].position", light->position);
      illuminationProgram.setVec3f("lights[" + idx + "].direction", light->direction);
      illuminationProgram.setVec3f("lights[" + idx + "].color", light->color);
      illuminationProgram.setFloat("lights[" + idx + "].radius", light->radius);
      illuminationProgram.setInt("lights[" + idx + "].type", light->type);
    }
  }

  gBuffer->renderScreenQuad();
}

void OpenGLVideoController::renderNonIlluminatedSurfaces() {
  auto& albedoProgram = gBuffer->getShaderProgram(GBuffer::Shader::ALBEDO);

  albedoProgram.use();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

  gBuffer->renderScreenQuad();
}

void OpenGLVideoController::renderScreenShaders() {
  glDisable(GL_STENCIL_TEST);

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

void OpenGLVideoController::renderShadowCasters() {
  auto& lightViewProgram = gBuffer->getShaderProgram(GBuffer::Shader::LIGHT_VIEW);
  auto& shadowCasterProgram = gBuffer->getShaderProgram(GBuffer::Shader::SHADOW_CASTER);

  for (auto* glShadowCaster : glShadowCasters) {
    // Shadowcaster light view pass
    lightViewProgram.use();

    gBuffer->startWriting();
    gBuffer->clearLightViewBuffers();

    const Camera& camera = scene->getCamera();
    auto* light = glShadowCaster->getLight();
    bool isDirectionalLight = light->type == Light::LightType::DIRECTIONAL;

    glDisable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Matrix4 lightMatrixCascades[] = {
      glShadowCaster->getLightMatrixCascade(0, camera),
      glShadowCaster->getLightMatrixCascade(1, camera),
      glShadowCaster->getLightMatrixCascade(2, camera)
    };

    int totalCascades = isDirectionalLight ? 3 : 1;

    for (int i = 0; i < totalCascades; i++) {
      glClear(GL_DEPTH_BUFFER_BIT);

      gBuffer->writeToShadowCascade(i);
      lightViewProgram.setMatrix4("lightMatrix", lightMatrixCascades[i]);

      for (auto* glObject : glObjects) {
        lightViewProgram.setMatrix4("modelMatrix", glObject->getSourceObject()->getMatrix());
        glObject->render();
      }
    }

    if (!isDirectionalLight) {
      gBuffer->useFirstShadowCascade();
    }

    // Camera view shadowcaster lighting pass
    screenShaders[0]->startWriting();
    gBuffer->startReading();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);

    shadowCasterProgram.use();
    shadowCasterProgram.setInt("colorTexture", 0);
    shadowCasterProgram.setInt("normalDepthTexture", 1);
    shadowCasterProgram.setInt("positionTexture", 2);
    shadowCasterProgram.setInt("lightMaps[0]", 3);
    shadowCasterProgram.setInt("lightMaps[1]", 4);
    shadowCasterProgram.setInt("lightMaps[2]", 5);
    shadowCasterProgram.setMatrix4("lightMatrixCascades[0]", lightMatrixCascades[0]);
    shadowCasterProgram.setMatrix4("lightMatrixCascades[1]", lightMatrixCascades[1]);
    shadowCasterProgram.setMatrix4("lightMatrixCascades[2]", lightMatrixCascades[2]);
    shadowCasterProgram.setVec3f("cameraPosition", camera.position);
    shadowCasterProgram.setVec3f("light.position", light->position);
    shadowCasterProgram.setVec3f("light.direction", light->direction);
    shadowCasterProgram.setVec3f("light.color", light->color);
    shadowCasterProgram.setFloat("light.radius", light->radius);
    shadowCasterProgram.setInt("light.type", light->type);

    gBuffer->renderScreenQuad();
  }

  glDisable(GL_BLEND);
}