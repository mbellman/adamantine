#version 330 core

uniform int hasTexture = 0;
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
  if (hasTexture == 1) {
    color = texture(modelTexture, fragmentUv).xyz;
  } else {
    color = fragmentColor;
  }

  normalDepth = vec4(normalize(fragmentNormal), getDepth());
  position = fragmentPosition;
}