#version 330 core

uniform sampler2D screen;

in vec2 fragmentUv;

layout (location = 0) out vec3 baseColor;
layout (location = 1) out vec3 bloomColor;

bool shouldApplyBloom(vec3 color) {
  return (
    color.r > 0.999 ||
    color.g > 0.999 ||
    color.b > 0.999
  );
}

void main() {
  vec3 screenColor = texture(screen, fragmentUv).xyz;

  baseColor = screenColor;

  if (shouldApplyBloom(screenColor)) {
    vec3 color = normalize(screenColor);

    bloomColor = color * color;
  } else {
    bloomColor = vec3(0.0);
  }
}