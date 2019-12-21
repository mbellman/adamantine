#include <string>
#include <fstream>

#include "glew.h"
#include "opengl/ShaderLoader.h"
#include "subsystem/FileLoader.h"

GLuint ShaderLoader::load(GLenum shaderType, const char* path) {
  GLuint shader = glCreateShader(shaderType);

  std::string source = FileLoader::load(path);
  const GLchar* shaderSource = source.c_str();

  glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    char error[512];

    glGetShaderInfoLog(shader, 512, 0, error);
    printf("Failed to compile shader!");
    printf(error);
  }

  return shader;
}

GLuint ShaderLoader::loadFragmentShader(const char* path) {
  return load(GL_FRAGMENT_SHADER, path);
}

GLuint ShaderLoader::loadVertexShader(const char* path) {
  return load(GL_VERTEX_SHADER, path);
}