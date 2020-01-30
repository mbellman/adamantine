#version 330 core

uniform sampler2D depthTexture;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

void main() {
  color = vec3(texture(depthTexture, fragmentUv).r);
}