#pragma once

#include "glew.h"
#include "glut.h"

template<typename T>
struct VertexShaderInput {
  const char* name;
  GLint size;
  GLenum type;
  int stride;
  int offset;
};

class ShaderProgram {
public:
  ~ShaderProgram();

  void activate();
  void attachShader(GLuint shader);
  void create();
  GLint getUniformLocation(const char* name);
  void setFragmentShaderOutput(const char* name);

  template<typename T>
  void setVertexShaderInput(const VertexShaderInput<T>& input) {
    GLint attribute = glGetAttribLocation(program, input.name);

    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, input.size, input.type, GL_FALSE, input.stride*sizeof(T), (void*)(input.offset*sizeof(T)));
  }

private:
  GLuint program = -1;
};