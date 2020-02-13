#version 330 core

#define MAX_LIGHTS 128

#include <adamantine/shaders/helpers/lighting.glsl>

const int POINT_LIGHT = 0;
const int DIRECTIONAL_LIGHT = 1;
const int SPOT_LIGHT = 2;

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform vec3 cameraPosition;
uniform int totalLights;
uniform Light lights[MAX_LIGHTS];

in vec2 fragmentUv;

layout (location = 0) out vec4 colorDepth;

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec4 normalDepth = texture(normalDepthTexture, fragmentUv);
  vec3 surfaceToCamera = normalize(cameraPosition - position);
  vec3 normal = normalDepth.xyz;
  vec3 combinedLighting = vec3(0.0);

  for (int i = 0; i < totalLights; i++) {
    Light light = lights[i];

    switch (light.type) {
      case POINT_LIGHT:
        combinedLighting += albedo * getPointLightFactor(lights[i], position, normal, surfaceToCamera);
        break;
      case DIRECTIONAL_LIGHT:
        combinedLighting += albedo * getDirectionalLightFactor(lights[i], normal, surfaceToCamera);
        break;
      case SPOT_LIGHT:
        combinedLighting += albedo * getSpotLightFactor(lights[i], position, normal, surfaceToCamera);
        break;
    }
  }

  colorDepth = vec4(combinedLighting, normalDepth.w);
}