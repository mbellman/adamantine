#version 330 core

uniform bool hasTexture = false;
uniform sampler2D modelTexture;

in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentPosition;
in vec2 fragmentUv;

layout (location = 0) out vec3 color;
layout (location = 1) out vec4 normalDepth;
layout (location = 2) out vec3 position;

float getDepth() {
  return gl_FragCoord.z / gl_FragCoord.w;
}

void main() {
  color = hasTexture
    ? texture(modelTexture, fragmentUv).xyz
    : fragmentColor;

  normalDepth = vec4(normalize(fragmentNormal), getDepth());
  position = fragmentPosition;
}