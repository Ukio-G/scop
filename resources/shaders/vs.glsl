#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Color;
} vs_out;

flat out vec3 flat_color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;
uniform vec3 viewPos;

uniform float u_time;

void main()
{
    vs_out.FragPos = vec4(transform * vec4(aPos, 1)).xyz;
    vs_out.Color = aColor;
    flat_color = aColor;
    gl_Position = projection * view * transform * vec4(aPos.xyz, 1.0);
}