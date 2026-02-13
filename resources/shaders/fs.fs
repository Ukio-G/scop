#version 330 core


layout(std140) uniform ObjectData {
    mat4 modelMatrix;
    ivec4 objectFlags;
} objData;

layout(std140) uniform FrameData {
     mat4   projection;
     mat4   view;
     mat4   transform;
     vec4   viewPos;

     float  textureColorLerpFactor;
     int   flatShading;
     int   grayscale;
} frameData;

uniform sampler2D diffuseMap;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
  vec3 Color;
} fs_in;

flat in vec3 flat_color;
out vec4 FragColor;
in vec3 vWorldPos;

vec3 fastNormal(vec3 p) {
    vec3 dx = dFdx(p);
    vec3 dy = dFdy(p);
    return normalize(cross(dx, dy));
}

vec3 sampleTriplanar(vec3 pos, vec3 norm) {
    vec3 n = abs(normalize(norm));
    n = (n - 0.2) / 0.8;
    n = max(n, 0.0);
    n /= dot(n, vec3(1.0));

    vec3 x = texture(diffuseMap, pos.yz).rgb;
    vec3 y = texture(diffuseMap, pos.zx).rgb;
    vec3 z = texture(diffuseMap, pos.xy).rgb;
    return x * n.x + y * n.y + z * n.z;
}


void main() {
    vec3 fast_normal = fastNormal(fs_in.FragPos);

    vec3 triplanarTextureColor = vec3(0, 0, 0);
    if (objData.objectFlags.x > 0)
        triplanarTextureColor = sampleTriplanar(fs_in.FragPos * 4.0, fast_normal);

    vec3 vertexColor = vec3(0);
    if (frameData.flatShading > 0)
        vertexColor = flat_color;
    else
        vertexColor = fs_in.Color;
    
    
    vec3 resultColor = vec3(0);

    if (objData.objectFlags.x > 0)
    {
        resultColor = mix(triplanarTextureColor, vertexColor, frameData.textureColorLerpFactor);
    }
    else
    {
        resultColor = vertexColor;
    }

    if (frameData.grayscale > 0) {
        float graylevel = (resultColor[0] + resultColor[1] + resultColor[2]) / 3;
        resultColor[0] = graylevel;
        resultColor[1] = graylevel;
        resultColor[2] = graylevel;
    }

    FragColor = vec4(resultColor, 1.0);
}
