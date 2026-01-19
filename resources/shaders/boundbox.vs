#version 330 core

layout(location=0) in vec3 aPos;


uniform mat4 projection;
uniform mat4 view;
// uniform mat4 transform;
// uniform vec3 viewPos;

uniform mat4 uModel;                   // матрица модели объекта
uniform mat4 uBboxScale;                   // матрица модели объекта

void main() {
    // float d = determinant(uBboxScale);
    // mat4 boxScale = uBboxScale / d;
    gl_Position = projection * view * uModel * uBboxScale * vec4(aPos, 1.0);
}