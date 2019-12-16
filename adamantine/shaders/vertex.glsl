#version 140

uniform float time;
in vec2 position;

void main() {
  float x = position.x * cos(time) - position.y * sin(time);
  float y = position.x * sin(time) + position.y * cos(time);

  gl_Position = vec4(x, y, 0, 1);
}