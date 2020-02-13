#version 330 core

#include <adamantine/shaders/helpers/blur.glsl>

uniform sampler2D baseColor;
uniform sampler2D bloomColor;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

void main() {
  color = texture(baseColor, fragmentUv).xyz + getBlur(bloomColor, fragmentUv, 30, vec2(0.0, 1.0));
}