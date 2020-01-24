#version 330 core

#define MAX_LIGHTS 256

struct Light {
  vec3 position;
  vec3 color;
  float radius;
};

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform vec3 cameraPosition;
uniform int totalLights;
uniform Light lights[MAX_LIGHTS];

in vec2 fragmentUv;

layout (location = 0) out vec4 color;

vec3 getLightFactor(Light light, vec3 surfacePosition, vec3 surfaceNormal) {
  vec3 surfaceToCamera = cameraPosition - surfacePosition;
  vec3 surfaceToLight = light.position - surfacePosition;

  float lightDistance = length(surfaceToLight);
  float attenuation = pow(1.0 / lightDistance, 2);
  vec3 lighting = light.color * light.radius * attenuation;

  vec3 normalizedSurfaceToLight = surfaceToLight / lightDistance;
  float normalDot = dot(normalizedSurfaceToLight, surfaceNormal);
  float illuminance = max(normalDot, 0.0);
  vec3 diffuse = lighting * illuminance;

  vec3 halfVector = normalize(normalize(surfaceToCamera) + normalizedSurfaceToLight);
  float specularDot = dot(halfVector, surfaceNormal);
  float specularity = pow(max(specularDot, 0.0), 50);
  vec3 specular = lighting * specularity * 5.0;
  
  return diffuse + specular;
}

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec4 normalDepth = texture(normalDepthTexture, fragmentUv);
  vec3 normal = normalDepth.xyz;
  float depth = normalDepth.w;
  vec3 outColor = albedo * 0.01;

  for (int i = 0; i < totalLights; i++) {
    outColor += albedo * getLightFactor(lights[i], position, normal);
  }

  color = vec4(outColor, depth);
}