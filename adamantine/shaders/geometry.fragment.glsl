#version 330 core

uniform bool hasTexture = false;
uniform bool hasNormalMap = false;
uniform sampler2D modelTexture;
uniform sampler2D normalMap;

in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentTangent;
in vec3 fragmentPosition;
in vec2 fragmentUv;

layout (location = 0) out vec3 color;
layout (location = 1) out vec4 normalDepth;
layout (location = 2) out vec3 position;

vec3 getColor() {
  return hasTexture
    ? texture(modelTexture, fragmentUv).xyz
    : fragmentColor;
}

mat3 getTBNMatrix() {
  vec3 surfaceNormal = normalize(fragmentNormal);
  vec3 surfaceTangent = normalize(fragmentTangent);

  surfaceTangent = normalize(surfaceTangent - dot(surfaceTangent, surfaceNormal) * surfaceNormal);

  vec3 surfaceBitangent = cross(surfaceTangent, surfaceNormal);

  return mat3(surfaceTangent, surfaceBitangent, surfaceNormal);
}

vec3 getNormal() {
  if (hasNormalMap) {
    vec3 mappedNormal = texture(normalMap, fragmentUv).xyz * 2.0 - vec3(1.0);
    mat3 matrix = getTBNMatrix();

    return normalize(matrix * mappedNormal);
  } else {
    return normalize(fragmentNormal);
  }
}

float getDepth() {
  return gl_FragCoord.z / gl_FragCoord.w;
}

void main() {
  color = getColor();
  normalDepth = vec4(getNormal(), getDepth());
  position = fragmentPosition;
}