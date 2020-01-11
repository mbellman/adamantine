#version 330 core

in vec3 fragmentColor;
in vec3 fragmentNormal;

layout (location = 0) out vec3 color;
layout (location = 1) out vec4 normalDepth;

float depth() {
  return gl_FragCoord.z / gl_FragCoord.w;
}

void main() {
  color = fragmentColor;
  normalDepth = vec4(fragmentNormal, depth());
}