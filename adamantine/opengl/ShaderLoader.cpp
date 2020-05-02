#include <string>
#include <fstream>
#include <cstdio>

#include "glew.h"
#include "opengl/ShaderLoader.h"
#include "subsystem/FileLoader.h"

GLuint ShaderLoader::load(GLenum shaderType, const char* path) {
  GLuint shader = glCreateShader(shaderType);

  std::string source = FileLoader::load(path);
  std::size_t includeStart;

  while ((includeStart = source.find("#include <")) != std::string::npos) {
    int pathStart = includeStart + 10;
    int pathEnd = source.find(">", pathStart) - pathStart;
    int includeEnd = source.find("\n", includeStart) - includeStart;
    std::string path = source.substr(pathStart, pathEnd);
    std::string includeSource = FileLoader::load(path.c_str());

    source.replace(includeStart, includeEnd, includeSource);
  }

  const GLchar* shaderSource = source.c_str();

  glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    char error[512];

    glGetShaderInfoLog(shader, 512, 0, error);
    printf("Failed to compile shader: %s\n", path);
    printf("%s\n", error);
  }

  return shader;
}

GLuint ShaderLoader::loadFragmentShader(const char* path) {
  return load(GL_FRAGMENT_SHADER, path);
}

GLuint ShaderLoader::loadGeometryShader(const char* path) {
  return load(GL_GEOMETRY_SHADER, path);
}

GLuint ShaderLoader::loadVertexShader(const char* path) {
  return load(GL_VERTEX_SHADER, path);
}