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
};

const Light lights[5] = Light[5](
  Light(
    vec3(50.0, 15.0, 250.0),
    vec3(1.0, 0.0, 1.0),
    750.0
  ),
  Light(
    vec3(-20.0, 10.0, 0.0),
    vec3(1.0, 0.0, 0.0),
    300.0
  ),
  Light(
    vec3(150.0, 50.0, 200.0),
    vec3(0.0, 1.0, 1.0),
    1000.0
  ),
  Light(
    vec3(-200.0, 15.0, -150.0),
    vec3(1.0, 1.0, 0.0),
    400.0
  ),
  Light(
    vec3(-100.0, 10.0, 300.0),
    vec3(0.0, 1.0, 0.0),
    500.0
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

vec3 getLightFactor(Light light, vec3 surfacePosition, vec3 surfaceNormal) {
  vec3 surfaceToCamera = cameraPosition - surfacePosition;
  vec3 surfaceToLight = light.position - surfacePosition;

  float lightDistance = length(surfaceToLight);
  float attenuation = pow(1.0 / lightDistance, 2);
  vec3 lighting = light.color * light.radius * attenuation;

  float normalDot = dot(surfaceToLight / lightDistance, surfaceNormal);
  float illuminance = max(normalDot, 0.0);
  vec3 diffuse = lighting * illuminance;

  vec3 halfVector = normalize(surfaceToCamera + surfaceToLight);
  float specularDot = dot(halfVector, surfaceNormal);
  float specularity = pow(max(specularDot, 0.0), 50);
  vec3 specular = lighting * specularity * 15.0 * illuminance;
  
  return diffuse + specular;
}

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec3 normal = texture(normalDepthTexture, fragmentUv).xyz;
  vec3 outColor = albedo * 0.0;

  for (int i = 0; i < lights.length(); i++) {
    outColor += albedo * getLightFactor(lights[i], position, normal);
  }

  color = outColor;
}