vec3 getBlur(sampler2D map, vec2 uv, int samples, vec2 direction) {
  vec4 blur = vec4(0.0);
  vec2 texelSize = 1.0 / textureSize(map, 0);
  int samples_h = samples / 2;

  for (int i = -samples_h; i <= samples_h; i++) {
    blur += texture(map, uv + direction * i * texelSize);
  }

  return vec3(blur / float(samples + 1));
}