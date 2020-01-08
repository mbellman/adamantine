#version 330 core

uniform sampler2D colorTexture;
uniform sampler2D normalDepthTexture;

in vec2 uv;

layout (location = 0) out vec3 color;

void main() {
  color = texture(colorTexture, uv).xyz;
}