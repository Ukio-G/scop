#version 330 core

uniform sampler2D diffuseMap;

uniform vec3 viewPos;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
  vec3 Color;
}
fs_in;

flat in vec3 flat_color;
out vec4 FragColor;

void main() {
    FragColor = vec4(flat_color , 1.0);
}
