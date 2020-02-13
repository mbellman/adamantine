struct Light {
  vec3 position;
  vec3 direction;
  vec3 color;
  float radius;
  int type;
};

vec3 getDirectionalLightFactor(Light light, vec3 surfaceNormal, vec3 surfaceToCamera) {
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

vec3 getPointLightFactor(Light light, vec3 surfacePosition, vec3 surfaceNormal, vec3 surfaceToCamera) {
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

vec3 getSpotLightFactor(Light light, vec3 surfacePosition, vec3 surfaceNormal, vec3 surfaceToCamera) {
  vec3 surfaceToLight = light.position - surfacePosition;
  float directionDot = max(dot(normalize(surfaceToLight), normalize(light.direction) * -1.0), 0.0);
  float shine = directionDot >= 0.8 ? 1.0 : pow(directionDot + 0.2, 30);

  return getPointLightFactor(light, surfacePosition, surfaceNormal, surfaceToCamera) * shine;
}