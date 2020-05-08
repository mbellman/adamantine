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

float createRadialWave(float x, float y, float frequency, float offset) {
  const float PI = 3.141592;
  const float HALF_PI = PI * 0.5;

  float cx = 2.0 * (x - 0.5);
  float cy = 2.0 * (y - 0.5);
  float radius = sqrt(cx * cx + cy * cy);

  return sin(cx * HALF_PI + radius * PI * frequency + offset) * cos(cy * HALF_PI);
}

vec3 getNormal() {
  float u = fragmentUv.x;
  float v = fragmentUv.y;

  float x = (
    createRadialWave(u, v, 10.0, -time) * 0.3 +
    createRadialWave(u + 0.5, v - 0.3, 10.0, -time) * 0.6 +
    sin((u + (sin(v * 10.0) + sin(v * (6.0 + v))) * 0.05) * 20.0) * sin(time * 2.0) * 0.2 +
    sin((u + sin(v * 5.0) * 0.1) * 200.0 + time * 3.0) * 0.01
  );

  float y = (
    createRadialWave(v, u, 10.0, -time) * 0.3 +
    createRadialWave(v + 0.5, u - 0.3, 10.0, -time) * 0.6 +
    sin((v + (sin(u * 10.0) + sin(u * (3.7 + u))) * 0.05) * 20.0) * cos(time * 2.0) * 0.2 +
    sin(v * 70.0 + time * 3.0) * 0.2 +
    sin((v + sin(u * 5.0) * 0.1) * 200.0 + time * 5.0) * 0.2 +
    cos((v + sin(u * 10.0) * 0.025) * 300.0 + time * 5.0) * 0.01
  );

  vec3 bumpMappedNormal = (texture(normalMap, fragmentUv + vec2(time * 0.01)).xyz * 2.0 - vec3(1.0)) * 2.0;

  vec3 normal = vec3(
    x,
    y,
    1.0
  ) + bumpMappedNormal;

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