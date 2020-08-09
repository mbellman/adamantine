float min9(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
  return min(a, min(b, min(c, min(d, min(e, min(f, min(g, min(h, i))))))));
}

const vec2 SAMPLE_OFFSETS[9] = vec2[9](
  vec2(-0.7, -0.7),
  vec2(0.0, -1.0),
  vec2(0.7, -0.7),
  vec2(-1.0, 0.0),
  vec2(0.0),
  vec2(1.0, 0.0),
  vec2(-0.7, 0.7),
  vec2(0.0, 1.0),
  vec2(0.7, 0.7)
);

const float MAX_SOFTNESS = 50.0;

float getShadowFactor(vec3 worldPosition, mat4 lightMatrix, sampler2D lightMap, float bias) {
  vec4 lightSpacePosition = lightMatrix * vec4(worldPosition * vec3(1.0, 1.0, -1.0), 1.0);
  vec3 projection = (lightSpacePosition.xyz / lightSpacePosition.w) * 0.5 + 0.5;

  if (projection.z >= 1.0) {
    return 1.0;
  }

  vec2 texelSize = 0.5 * 1.0 / textureSize(lightMap, 0);
  vec2 sampleSpread = texelSize * MAX_SOFTNESS;

  float occludingSurfaceDistance = min9(
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[0] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[1] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[2] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[3] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[4] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[5] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[6] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[7] * sampleSpread).r,
    texture(lightMap, projection.xy + SAMPLE_OFFSETS[8] * sampleSpread).r
  );

  float shadowDistance = projection.z - occludingSurfaceDistance;
  float blur = 1.0 + MAX_SOFTNESS * shadowDistance;
  float shadowSum = 0.0;

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      float closestDepth = texture(lightMap, projection.xy + vec2(x, y) * texelSize * blur).r;

      shadowSum += (closestDepth < projection.z - bias) ? 0.0 : 1.0;
    }
  }

  return min(1.75 * shadowSum / 25.0, 1.0);
}