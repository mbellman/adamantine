#version 330 core

uniform mat4 modelMatrix;

in vec3 vertexPosition;

void main() {
  gl_Position = modelMatrix * vec4(vertexPosition, 1.0);
}