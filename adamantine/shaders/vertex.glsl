#version 140

uniform float time;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexUv;

out vec3 fragmentColor;

void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

  mat3 normMatrix = transpose(inverse(mat3(modelMatrix)));

  fragmentColor = normalize(normMatrix * vertexColor);
}