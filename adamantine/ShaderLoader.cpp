#include <string>
#include <fstream>

#include "glew.h"
#include "ShaderLoader.h"
#include "FileLoader.h"

GLuint ShaderLoader::load(GLenum shaderType, const char* path) {
  GLuint shader = glCreateShader(shaderType);

  std::string source = FileLoader::load(path);
  const GLchar* shaderSource = source.c_str();

  glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);
  glCompileShader(shader);

  return shader;
}