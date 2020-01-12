#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform vec3 cameraPosition;

in vec2 fragmentUv;

layout (location = 0) out vec3 color;

struct Light {
  vec3 position;
  vec3 color;
  float radius;
  float intensity;
};

const Light lights[5] = Light[5](
  Light(
    vec3(0.0, 15.0, 250.0),
    vec3(1.0, 0.0, 1.0),
    50.0,
    1.0
  ),
  Light(
    vec3(-100.0, 10.0, 100.0),
    vec3(1.0, 0.0, 0.0),
    300.0,
    1.0
  ),
  Light(
    vec3(200.0, 50.0, -200.0),
    vec3(0.0, 1.0, 1.0),
    150.0,
    1.0
  ),
  Light(
    vec3(-200.0, 15.0, -150.0),
    vec3(1.0, 1.0, 0.0),
    50.0,
    1.0
  ),
  Light(
    vec3(-100.0, 10.0, 300.0),
    vec3(0.0, 1.0, 0.0),
    30.0,
    1.0
  )
);

float getBlurFactor(float depth) {
  float MIN_BLUR = 0.0;
  float MAX_BLUR = 0.0015;
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
  float ambientLightFactor = 0.01;
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec3 normal = texture(normalDepthTexture, fragmentUv).xyz;
  vec3 cameraToSurface = position - cameraPosition;
  vec3 baseColor = albedo * ambientLightFactor;
  vec3 lightSum = vec3(0.0);
  int total = 0;

  for (int i = 0; i < lights.length(); i++) {
    Light light = lights[i];

    vec3 lightToSurface = position - light.position;
    float lightToSurfaceDistance = length(lightToSurface);
    float lightIncidence = dot(normalize(lightToSurface), normal);

    if (lightIncidence < 0) {
      const float PI = 3.141592;
      vec3 litColor = mix(albedo, light.color, 0.5);
      float viewIncidence = dot(normalize(cameraToSurface), normal);
      float attenuation = pow(1.0 / lightToSurfaceDistance, 1.0 / 10.0);
      float strength = pow(1.0 - lightToSurfaceDistance / light.radius, 2.0);
      float illuminance = (1.0 + cos((1.0 + lightIncidence) * PI)) / 2.0;
      float glance = 1.0 - abs(viewIncidence);
      float intensity = pow(illuminance, 5) * attenuation * glance * 2.0;

      lightSum += mix(baseColor, litColor, intensity);
      total++;
    }
  }

  color = baseColor + lightSum / total;
}