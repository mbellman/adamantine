#version 330 core

layout (location = 3) out float depth;

void main() {
  depth = gl_FragCoord.z;
}