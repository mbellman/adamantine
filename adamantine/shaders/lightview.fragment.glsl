#version 330 core

in vec4 fragPosition;

layout (location = 3) out float depth;

void main() {
  depth = gl_FragCoord.z;
}