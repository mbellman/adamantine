#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrices[6];

out vec4 worldPosition;

void main() {
  for (int f = 0; f < 6; f++) {
    gl_Layer = f;

    for (int v = 0; v < 3; v++) {
      worldPosition = gl_in[v].gl_Position;
      gl_Position = lightMatrices[f] * worldPosition;

      EmitVertex();
    }
    
    EndPrimitive();
  }
}  