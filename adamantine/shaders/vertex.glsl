#version 140

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexUv;

out vec3 fragmentColor;

vec4 position() {
  return projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}

vec3 normal() {
  mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

  return normalize(normalMatrix * vertexNormal);
}

vec3 color() {
  return vertexColor;
}

vec2 uv() {
  return vertexUv;
}

void main() {
  gl_Position = position();
  fragmentColor = normal();
}