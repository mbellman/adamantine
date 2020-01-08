#pragma once

#include <vector>

#include "glew.h"
#include "glut.h"

struct VertexAttribute {
  const char* name;
  GLint size;
  GLenum type;
  unsigned int stride;
  unsigned int offset;
};

struct VertexShaderInput {
  const char* name;
  GLint size;
  GLenum type;
};

class ShaderProgram {
public:
  ~ShaderProgram();

  void activate();
  void attachShader(GLuint shader);
  void bindVertexInputs();
  void create();
  GLint getUniformLocation(const char* name);

  template<typename T>
  void saveVertexInputs(int number, const VertexShaderInput* inputs) {
    int stride = 0;
    int offset = 0;

    for (int i = 0; i < number; i++) {
      stride += inputs[i].size;
    }

    for (int i = 0; i < number; i++) {
      const VertexShaderInput& input = inputs[i];

      vertexAttributes.push_back({
        input.name,
        input.size,
        input.type,
        stride * sizeof(T),
        offset * sizeof(T)
      });

      offset += input.size;
    }
  }

  void setFragmentShaderOutput(const char* name);

private:
  GLuint program = -1;
  std::vector<VertexAttribute> vertexAttributes;

  void setVertexAttribute(const VertexAttribute& attribute);
};