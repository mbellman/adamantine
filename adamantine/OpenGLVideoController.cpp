#include "OpenGLVideoController.h"
#include "SDL.h"
#include "glew.h"
#include "SDL_opengl.h"
#include "glut.h"

SDL_Window* OpenGLVideoController::getWindow(const char* title, Region2d<int> region) {
  return SDL_CreateWindow(title, region.x, region.y, region.width, region.height, SDL_WINDOW_OPENGL);
}

void OpenGLVideoController::destroy() {
  SDL_GL_DeleteContext(glContext);
}

void OpenGLVideoController::init() {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  SDL_GL_SetSwapInterval(1);

  glContext = SDL_GL_CreateContext(sdlWindow);

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

  // Create vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

  const GLchar* vertexShaderSource[] =
  {
    "#version 140\nin vec2 position; void main() { gl_Position = vec4( position.x, position.y, 0, 1 ); }"
  };

  glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Create fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar* fragmentShaderSource[] =
  {
    "#version 140\nout vec4 color; void main() { color = vec4( 1.0, 0.0, 1.0, 1.0 ); }"
  };

  glShaderSource(fragmentShader, 1, fragmentShaderSource, 0);
  glCompileShader(fragmentShader);

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
}

void OpenGLVideoController::render() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  SDL_GL_SwapWindow(sdlWindow);
}