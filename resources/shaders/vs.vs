#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;

layout(std140) uniform ObjectData{
    mat4 modelMatrix;
} objData;

layout(std140) uniform SubspaceData {
    mat4 subspaceMatrix;
} subspaceData;

layout(std140) uniform FrameData {
     mat4   projection;
     mat4   view;
     mat4   transform;
     vec4   viewPos;

     int    hasTexture;
     float  textureColorLerpFactor;
     int   flatShading;
     int   grayscale;
} frameData;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Color;
} vs_out;

flat out vec3 flat_color;

void main()
{
    vs_out.FragPos = vec4(subspaceData.subspaceMatrix * objData.modelMatrix * vec4(aPos, 1)).xyz;
    vs_out.Color = aColor;
    flat_color = aColor;
    gl_Position = frameData.projection * frameData.view * subspaceData.subspaceMatrix * objData.modelMatrix * vec4(aPos.xyz, 1.0);
}
