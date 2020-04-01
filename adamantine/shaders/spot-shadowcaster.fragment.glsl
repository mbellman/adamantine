#version 330 core

#include <adamantine/shaders/helpers/lighting.glsl>
#include <adamantine/shaders/helpers/shadows.glsl>

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform sampler2D lightMap;
uniform mat4 lightMatrix;
uniform vec3 cameraPosition;
uniform Light light;

in vec2 fragmentUv;

layout (location = 0) out vec4 colorDepth;

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec4 normalDepth = texture(normalDepthTexture, fragmentUv);
  vec3 surfaceToCamera = normalize(cameraPosition - position);
  vec3 normal = normalDepth.xyz;
  vec3 lighting = albedo * getSpotLightFactor(light, position, normal, surfaceToCamera);
  float shadowFactor = getShadowFactor(position, lightMatrix, lightMap, 0.0001);

  colorDepth = vec4(lighting * shadowFactor, normalDepth.w);
}