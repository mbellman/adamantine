#version 330 core

uniform sampler2D baseColor;
uniform sampler2D bloomColor;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

vec3 getBloom() {
  vec4 bloom = vec4(0.0);
  vec2 texelSize = 1.0 / textureSize(bloomColor, 0);

  for (int y = -15; y <= 15; y++) {
    bloom += texture(bloomColor, fragmentUv + vec2(0.0, y * texelSize.y));
  }

  return (bloom / 31.0).xyz;
}

void main() {
  color = texture(baseColor, fragmentUv).xyz + getBloom();
}