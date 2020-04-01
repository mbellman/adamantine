#version 330 core

#include <adamantine/shaders/helpers/lighting.glsl>
#include <adamantine/shaders/helpers/shadows.glsl>

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

float getBias(int cascadeIndex) {
  switch (cascadeIndex) {
    case 0:
    case 1:
      return 0.0001;
    case 2:
      return 0.0002;
  }
}

void main() {
  vec3 albedo = texture(colorTexture, fragmentUv).xyz;
  vec3 position = texture(positionTexture, fragmentUv).xyz;
  vec4 normalDepth = texture(normalDepthTexture, fragmentUv);
  vec3 surfaceToCamera = normalize(cameraPosition - position);
  vec3 normal = normalDepth.xyz;
  vec3 lighting = albedo * getDirectionalLightFactor(light, normal, surfaceToCamera);
  int cascadeIndex = getCascadeIndex(position);
  float shadowFactor = getShadowFactor(position, lightMatrixCascades[cascadeIndex], lightMaps[cascadeIndex], getBias(cascadeIndex));

  colorDepth = vec4(lighting * shadowFactor, normalDepth.w);
}