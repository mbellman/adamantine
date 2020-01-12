#version 330 core

in vec2 vertexPosition;
in vec2 vertexUv;

out vec2 fragmentUv;

void main() {
  gl_Position = vec4(vertexPosition, 0.0, 1.0);

  fragmentUv = vertexUv;
}