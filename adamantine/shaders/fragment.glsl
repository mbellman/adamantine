#version 140

uniform float time;
out vec4 color;

void main() {
  float r = abs(sin(time * 1.01));
  float g = abs(cos(time * 1.03));
  float b = abs(sin(time * 1.1));

  color = vec4(r, g, b, 1.0);
}