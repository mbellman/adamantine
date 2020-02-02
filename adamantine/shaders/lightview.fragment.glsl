#version 330 core

in vec4 fragPosition;

layout (location = 0) out float depth;

void main() {
  depth = gl_FragCoord.z;
}