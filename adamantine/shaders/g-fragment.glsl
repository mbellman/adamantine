#version 330 core

in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentPosition;

layout (location = 0) out vec3 color;
layout (location = 1) out vec4 normalDepth;
layout (location = 2) out vec3 position;

float getDepth() {
  return gl_FragCoord.z / gl_FragCoord.w;
}

void main() {
  color = fragmentColor;
  normalDepth = vec4(normalize(fragmentNormal), getDepth());
  position = fragmentPosition;
}