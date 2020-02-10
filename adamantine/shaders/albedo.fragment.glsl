#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;

in vec2 fragmentUv;

layout (location = 0) out vec4 colorDepth;

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  float depth = texture(normalDepthTexture, fragmentUv).w;

  colorDepth = vec4(albedo, depth);
}