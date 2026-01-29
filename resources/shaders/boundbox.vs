#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform ObjectData{
    mat4 modelMatrix;
} objData;

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

out vec3 Color;

uniform mat4 uBboxScale;                   // матрица модели объекта

void main() {
    gl_Position = frameData.projection * frameData.view * objData.modelMatrix * uBboxScale * vec4(aPos, 1.0);
    Color = aColor;
}