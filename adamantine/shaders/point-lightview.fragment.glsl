#version 330 core

uniform vec3 lightPosition;
uniform float farPlane;

in vec4 worldPosition;

void main() {
  gl_FragDepth = length(worldPosition.xyz - lightPosition) / farPlane;
}