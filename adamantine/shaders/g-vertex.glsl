#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexUv;

out vec3 fragmentColor;
out vec3 fragmentNormal;
out vec3 fragmentPosition;

vec4 clipPosition() {
  return projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}

vec3 worldPosition() {
  vec3 position = vec4(modelMatrix * vec4(vertexPosition, 1.0)).xyz;

  return vec3(position.x, position.y, -position.z);
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
  gl_Position = clipPosition();

  fragmentColor = color();
  fragmentNormal = normal();
  fragmentPosition = worldPosition();
}