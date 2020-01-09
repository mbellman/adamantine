#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

float getBlurFactor(float depth) {
  float MIN_BLUR = 0.0;
  float MAX_BLUR = 0.002;
  float MAX_BLUR_DEPTH = 500.0;

  float r = (depth * 10000.0) / MAX_BLUR_DEPTH;

  if (r >= 1.0) {
    return MAX_BLUR;
  }

  return mix(MIN_BLUR, MAX_BLUR, pow(r, 3));
}

void main() {
  // vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  // vec3 normal = texture(normalDepthTexture, fragmentUv).xyz;
  vec4 sum = vec4(0.0);
  float depth = texture(normalDepthTexture, fragmentUv).w;
  float blurFactor = getBlurFactor(depth);

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      sum += texture(
        colorTexture,
        vec2(fragmentUv.x + x * blurFactor, fragmentUv.y + y * blurFactor)
      ) / 25.0;
    }
  }

  color = sum.xyz;
}