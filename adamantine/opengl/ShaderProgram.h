#pragma once

#include <vector>
#include <string>

#include "glew.h"
#include "glut.h"
#include "subsystem/Math.h"

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
  void setBool(std::string name, bool value) const;
  void setFloat(std::string name, float value) const;
  void setInt(std::string name, int value) const;
  void setMatrix4(std::string name, const Matrix4& value) const;
  void setVec3f(std::string name, const Vec3f& value) const;

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

  void use() const;

private:
  GLuint program = -1;
  std::vector<VertexAttribute> vertexAttributes;

  void setVertexAttribute(const VertexAttribute& attribute);
};