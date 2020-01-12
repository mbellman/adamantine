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

vec4 getClipPosition() {
  return projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}

vec3 getWorldPosition() {
  vec3 position = vec4(modelMatrix * vec4(vertexPosition, 1.0)).xyz;

  return vec3(position.x, position.y, -position.z);
}

vec3 getNormal() {
  mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
  vec3 normal = normalize(normalMatrix * vertexNormal);

  return vec3(normal.x, normal.y, -normal.z);
}

vec3 getColor() {
  return vertexColor;
}

vec2 getUv() {
  return vertexUv;
}

void main() {
  gl_Position = getClipPosition();

  fragmentColor = getColor();
  fragmentNormal = getNormal();
  fragmentPosition = getWorldPosition();
}