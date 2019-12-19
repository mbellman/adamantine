#version 140

uniform float time;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 fragmentColor;

void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
  fragmentColor = vertexColor;
}