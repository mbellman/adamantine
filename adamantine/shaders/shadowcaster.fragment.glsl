#version 330 core

#include <adamantine/shaders/helpers/lighting.glsl>

const int POINT_LIGHT = 0;
const int DIRECTIONAL_LIGHT = 1;
const int SPOT_LIGHT = 2;

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;
uniform sampler2D positionTexture;
uniform sampler2D lightMaps[3];
uniform mat4 lightMatrixCascades[3];
uniform vec3 cameraPosition;
uniform Light light;

in vec2 fragmentUv;

layout (location = 0) out vec4 colorDepth;

int getCascadeIndex(vec3 position) {
  float fragDistance = length(cameraPosition - position);

  if (fragDistance < 150.0) {
    return 0;
  } else if (fragDistance < 500.0) {
    return 1;
  } else {
    return 2;
  }
}

float getShadowFactor(vec3 position) {
  int cascadeIndex = getCascadeIndex(position);
  vec4 lightSpacePosition = lightMatrixCascades[cascadeIndex] * vec4(position * vec3(1.0, 1.0, -1.0), 1.0);
  vec3 projection = (lightSpacePosition.xyz / lightSpacePosition.w) * 0.5 + 0.5;

  if (projection.z >= 1.0) {
    return 1.0;
  }

  float shadowFactor = 0.0;
  vec2 texelSize = 0.5 * 1.0 / textureSize(lightMaps[cascadeIndex], 0);
  float bias = light.type == DIRECTIONAL_LIGHT ? 0.001 : 0.00025;

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      float closestDepth = texture(lightMaps[cascadeIndex], projection.xy + vec2(x, y) * texelSize).r;

      shadowFactor += (closestDepth < projection.z - bias) ? 0.0 : 1.0;
    }
  }

  return shadowFactor / 25.0;
}

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec4 normalDepth = texture(normalDepthTexture, fragmentUv);
  vec3 surfaceToCamera = normalize(cameraPosition - position);
  vec3 normal = normalDepth.xyz;
  vec3 lighting = vec3(0.0);

  switch (light.type) {
    case POINT_LIGHT:
      lighting = albedo * getPointLightFactor(light, position, normal, surfaceToCamera);
      break;
    case DIRECTIONAL_LIGHT:
      lighting = albedo * getDirectionalLightFactor(light, normal, surfaceToCamera);
      break;
    case SPOT_LIGHT:
      lighting = albedo * getSpotLightFactor(light, position, normal, surfaceToCamera);
      break;
  }

  colorDepth = vec4(lighting * getShadowFactor(position), normalDepth.w);
}