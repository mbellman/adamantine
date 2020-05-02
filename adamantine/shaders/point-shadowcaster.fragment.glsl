#version 330 core

#include <adamantine/shaders/helpers/lighting.glsl>

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform samplerCube lightCubeMap;
uniform float farPlane;
uniform vec3 cameraPosition;
uniform Light light;

in vec2 fragmentUv;

layout (location = 0) out vec4 colorDepth;

float getPointShadowFactor(vec3 surfacePosition, vec3 surfaceNormal) {
  vec3 lightToSurface = surfacePosition - light.position;
  vec3 surfaceToLight = lightToSurface * -1.0;
  float closestDepth = texture(lightCubeMap, lightToSurface * vec3(1.0, 1.0, -1.0)).r * farPlane;
  float surfaceDistance = length(lightToSurface);
  float bias = 0.5 + (1.0 - dot(normalize(surfaceToLight), surfaceNormal)) * surfaceDistance * 0.2;

  return (closestDepth < surfaceDistance - bias) ? 0.0 : 1.0;
}

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 surfacePosition = texture(positionTexture, fragmentUv).xyz;
  vec4 normalDepth = texture(normalDepthTexture, fragmentUv);
  vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);
  vec3 normal = normalDepth.xyz;
  vec3 lighting = albedo * getPointLightFactor(light, surfacePosition, normal, surfaceToCamera);
  float shadowFactor = getPointShadowFactor(surfacePosition, normal);

  colorDepth = vec4(lighting * shadowFactor, normalDepth.w);
}