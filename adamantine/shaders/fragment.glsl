#version 140

uniform float time;

in vec3 fragmentColor;

out vec4 color;

void main() {
  color = vec4(fragmentColor, 1.0);
}