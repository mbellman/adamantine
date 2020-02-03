#version 330 core

uniform mat4 modelMatrix;
uniform mat4 lightMatrix;

in vec3 vertexPosition;

out vec4 fragPosition;

void main() {
  gl_Position = lightMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}