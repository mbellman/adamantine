#pragma once

#include <vector>
#include <string>

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

  void attachShader(GLuint shader);
  void bindVertexInputs();
  void create();
  GLint getUniformLocation(const char* name) const;
  GLint getUniformLocation(std::string name) const;
  void link();

  template<typename T>
  void setVertexInputs(int number, const VertexShaderInput* inputs) {
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
  void use() const;

private:
  GLuint program = -1;
  std::vector<VertexAttribute> vertexAttributes;

  void setVertexAttribute(const VertexAttribute& attribute);
};