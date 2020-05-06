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
  return vec3(1.0);
}

mat3 getTBNMatrix() {
  vec3 surfaceNormal = normalize(fragmentNormal);
  vec3 surfaceTangent = normalize(fragmentTangent);

  surfaceTangent = normalize(surfaceTangent - dot(surfaceTangent, surfaceNormal) * surfaceNormal);

  vec3 surfaceBitangent = cross(surfaceTangent, surfaceNormal);

  return mat3(surfaceTangent, surfaceBitangent, surfaceNormal);
}

vec3 getNormal() {
  float x = (
    sin((fragmentUv.x + sin(fragmentUv.y * 10.0) * 0.1) * 50.0) * sin(time * 2.0) * 0.3 +
    sin(fragmentUv.x * 70.0) * 0.2 +
    sin((fragmentUv.x + sin(fragmentUv.y * 5.0) * 0.1) * 200.0 + time * 3.0) * 0.1
  );

  float y = (
    sin((fragmentUv.y + sin(fragmentUv.x * 10.0) * 0.1) * 50.0) * cos(time * 2.0) * 0.3 +
    sin(fragmentUv.y * 70.0 + time * 3.0) * 0.2 +
    sin((fragmentUv.y + sin(fragmentUv.x * 5.0) * 0.1) * 200.0 + time * 5.0) * 0.1
  );

  vec3 normal = vec3(
    x,
    y,
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