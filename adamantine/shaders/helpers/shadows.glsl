float getShadowFactor(vec3 worldPosition, mat4 lightMatrix, sampler2D lightMap, float bias) {
  vec4 lightSpacePosition = lightMatrix * vec4(worldPosition * vec3(1.0, 1.0, -1.0), 1.0);
  vec3 projection = (lightSpacePosition.xyz / lightSpacePosition.w) * 0.5 + 0.5;

  if (projection.z >= 1.0) {
    return 1.0;
  }

  float shadowFactor = 0.0;
  vec2 texelSize = 0.5 * 1.0 / textureSize(lightMap, 0);

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      float closestDepth = texture(lightMap, projection.xy + vec2(x, y) * texelSize).r;

      shadowFactor += (closestDepth < projection.z - bias) ? 0.0 : 1.0;
    }
  }

  return shadowFactor / 25.0;
}