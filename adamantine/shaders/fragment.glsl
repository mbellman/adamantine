#version 140

uniform float time;

in vec3 fragmentColor;

out vec4 color;

const float HALF_PI = 3.141592 / 2.0;

void main() {
  // float r = abs(sin(fragmentColor.x + time));
  // float g = abs(cos(fragmentColor.y + time));
  // float b = abs(sin(fragmentColor.z + time + HALF_PI));

  color = vec4(fragmentColor, 1.0);
}