#version 330 core

#include <adamantine/shaders/helpers/blur.glsl>

uniform sampler2D baseColor;
uniform sampler2D bloomColor;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;
layout (location = 1) out vec3 bloomOutColor;

void main() {
  color = texture(baseColor, fragmentUv).xyz;
  bloomOutColor = getBlur(bloomColor, fragmentUv, 30, vec2(1.0, 0.0));
}