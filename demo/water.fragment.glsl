#version 330 core

uniform bool hasTexture = false;
uniform bool hasNormalMap = false;
uniform sampler2D modelTexture;
uniform sampler2D normalMap;
uniform float time;

in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentTangent;
in vec3 fragmentPosition;
in vec2 fragmentUv;

layout (location = 0) out vec3 color;
layout (location = 1) out vec4 normalDepth;
layout (location = 2) out vec3 position;

vec3 getColor() {
  return vec3(0.0, 0.25, 1.0);
}

mat3 getTBNMatrix() {
  vec3 surfaceNormal = normalize(fragmentNormal);
  vec3 surfaceTangent = normalize(fragmentTangent);

  surfaceTangent = normalize(surfaceTangent - dot(surfaceTangent, surfaceNormal) * surfaceNormal);

  vec3 surfaceBitangent = cross(surfaceTangent, surfaceNormal);

  return mat3(surfaceTangent, surfaceBitangent, surfaceNormal);
}

vec3 getNormal() {
  float t = sin(fragmentUv.x * 100.0);
  float x = sin(fragmentUv.y * 200.0);

  vec3 normal = vec3(
    0.25 * t * sin(fragmentUv.y * 50.0),
    0.0,
    1.0
  );

  return normalize(getTBNMatrix() * normal);
}

float getDepth() {
  return gl_FragCoord.z / gl_FragCoord.w;
}

void main() {
  color = getColor();
  normalDepth = vec4(getNormal(), getDepth());
  position = fragmentPosition;
}