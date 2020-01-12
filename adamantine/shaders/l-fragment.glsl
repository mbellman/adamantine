#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform vec3 cameraPosition;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

float getBlurFactor(float depth) {
  float MIN_BLUR = 0.0;
  float MAX_BLUR = 0.002;
  float MAX_BLUR_DEPTH = 300.0;

  float r = depth / MAX_BLUR_DEPTH;

  if (r >= 1.0) {
    return MAX_BLUR;
  }

  return mix(MIN_BLUR, MAX_BLUR, pow(r, 3));
}

vec4 getDoF() {
  vec4 sum = vec4(0.0);
  float depth = texture(normalDepthTexture, fragmentUv).w;
  float blur = getBlurFactor(depth);

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      vec2 sampleUv = vec2(fragmentUv.x + x * blur, fragmentUv.y + y * blur);

      sum += texture(colorTexture, sampleUv) / 25.0;
    }
  }

  return sum;
}

void main() {
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec3 relativePosition = position - cameraPosition;
  float mag = length(relativePosition);

  color = getDoF().xyz * clamp(mix(0.0, 1.0, 1 - mag / 500.0), 0.0, 1.0);
}