#version 330 core

struct Light {
  vec3 position;
  vec3 direction;
  vec3 color;
  float radius;
  int type;
};

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

  for (int x = -2; x <= 2; x++) {
    for (int y = -2; y <= 2; y++) {
      float closestDepth = texture(lightMaps[cascadeIndex], projection.xy + vec2(x, y) * texelSize).r;

      shadowFactor += (closestDepth < projection.z - 0.001) ? 0.0 : 1.0;
    }
  }

  return shadowFactor / 25.0;
}

vec3 getDirectionalLightFactor(vec3 surfaceNormal, vec3 surfaceToCamera) {
  vec3 surfaceToLight = -1.0 * normalize(light.direction);

  // Diffuse term
  float normalDot = dot(surfaceToLight, surfaceNormal);
  vec3 diffuse = light.color * max(normalDot, 0.0);

  // Specular term
  vec3 halfVector = normalize(surfaceToCamera + surfaceToLight);
  float specularDot = dot(halfVector, surfaceNormal);
  float specularity = pow(max(specularDot, 0.0), 30);
  vec3 specular = light.color * specularity;

  return diffuse + specular;
}

vec3 getPointLightFactor(vec3 surfacePosition, vec3 surfaceNormal, vec3 surfaceToCamera) {
  vec3 surfaceToLight = light.position - surfacePosition;

  float lightDistance = length(surfaceToLight);
  float attenuation = pow(1.0 / lightDistance, 2);
  vec3 lighting = light.color * light.radius * attenuation;

  // Diffuse term
  vec3 normalizedSurfaceToLight = surfaceToLight / lightDistance;
  float normalDot = dot(normalizedSurfaceToLight, surfaceNormal);
  float illuminance = max(normalDot, 0.0);
  vec3 diffuse = lighting * illuminance;

  // Specular term
  vec3 halfVector = normalize(surfaceToCamera + normalizedSurfaceToLight);
  float specularDot = dot(halfVector, surfaceNormal);
  float specularity = pow(max(specularDot, 0.0), 50);
  vec3 specular = lighting * specularity * 5.0;
  
  return diffuse + specular;
}

vec3 getSpotLightFactor(vec3 surfacePosition, vec3 surfaceNormal, vec3 surfaceToCamera) {
  vec3 surfaceToLight = light.position - surfacePosition;
  float directionDot = max(dot(normalize(surfaceToLight), normalize(light.direction) * -1.0), 0.0);
  float shine = directionDot >= 0.8 ? 1.0 : pow(directionDot + 0.2, 30);

  return getPointLightFactor(surfacePosition, surfaceNormal, surfaceToCamera) * shine;
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
      lighting = albedo * getPointLightFactor(position, normal, surfaceToCamera);
      break;
    case DIRECTIONAL_LIGHT:
      lighting = albedo * getDirectionalLightFactor(normal, surfaceToCamera);
      break;
    case SPOT_LIGHT:
      lighting = albedo * getSpotLightFactor(position, normal, surfaceToCamera);
      break;
  }

  colorDepth = vec4(lighting * getShadowFactor(position), normalDepth.w);
}