#version 330 core

uniform sampler2D baseColor;
uniform sampler2D bloomColor;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;
layout (location = 1) out vec3 bloomOutColor;

vec3 getBloom() {
  vec4 bloom = vec4(0.0);
  vec2 texelSize = 1.0 / textureSize(bloomColor, 0);

  for (int x = -15; x <= 15; x++) {
    bloom += texture(bloomColor, fragmentUv + vec2(x * texelSize.x, 0.0));
  }

  return (bloom / 31.0).xyz;
}

void main() {
  color = texture(baseColor, fragmentUv).xyz;
  bloomOutColor = getBloom();
}