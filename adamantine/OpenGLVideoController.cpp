#include <cmath>
#include <cstdio>

#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"
#include "OpenGLVideoController.h"
#include "ShaderLoader.h"

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

  // Vertices
  float vertices[] = {
    0.0f,  0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
  };

  // Create VBO
  GLuint vbo;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create VAO
  GLuint vao;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create shaders
  GLuint vertexShader = ShaderLoader::loadVertexShader("./shaders/vertex.glsl");
  GLuint fragmentShader = ShaderLoader::loadFragmentShader("./shaders/fragment.glsl");

  // Combine shaders
  GLuint glProgram = glCreateProgram();

  glAttachShader(glProgram, vertexShader);
  glAttachShader(glProgram, fragmentShader);
  
  // Fragment data output
  glBindFragDataLocation(glProgram, 0, "color");

  glLinkProgram(glProgram);
  glUseProgram(glProgram);

  GLint positionAttribute = glGetAttribLocation(glProgram, "position");

  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(positionAttribute);

  time = glGetUniformLocation(glProgram, "time");
}

void OpenGLVideoController::onRender() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform1f(time, SDL_GetTicks() / 500.0f);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  SDL_GL_SwapWindow(sdlWindow);
  glFinish();
}