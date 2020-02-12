#version 330 core

uniform sampler2D screen;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

vec2 getBlur(float depth) {
  vec2 MIN_BLUR = vec2(0.0);
  vec2 MAX_BLUR = 1.0 / textureSize(screen, 0);
  float MAX_BLUR_DEPTH = 200.0;

  float r = depth / MAX_BLUR_DEPTH;

  if (r >= 1.0) {
    return MAX_BLUR;
  }

  return mix(MIN_BLUR, MAX_BLUR, pow(r, 3));
}

vec4 getDoF() {
  vec4 sum = vec4(0.0);
  float depth = texture(screen, fragmentUv).w;
  vec2 blur = getBlur(depth);

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      sum += texture(screen, fragmentUv + vec2(x, y) * blur);
    }
  }

  return sum / 25.0;
}

void main() {
  color = getDoF().xyz;
}