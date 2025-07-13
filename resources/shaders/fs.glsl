#version 330 core

uniform sampler2D diffuseMap;
uniform vec3 viewPos;
uniform float u_time;
uniform int u_hasTexture;
uniform float u_textureColorLerpFactor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
  vec3 Color;
}
fs_in;

flat in vec3 flat_color;
out vec4 FragColor;
in vec3 vWorldPos;

vec3 fastNormal(vec3 p) {
    vec3 dx = dFdx(p);          // производные по экрану
    vec3 dy = dFdy(p);
    return normalize(cross(dx, dy));
}

vec3 sampleTriplanar(vec3 pos, vec3 norm) {
    vec3 n = abs(normalize(norm));
    n = (n - 0.2) / 0.8;           // небольшое «перо»
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
    if (u_hasTexture > 0)
        triplanarTextureColor = sampleTriplanar(fs_in.FragPos * 4.0, fast_normal);

    vec3 vertexColor = fs_in.Color;

    vec3 resultColor = vec3(0);

    if (u_hasTexture > 0)
    {
        resultColor = mix(triplanarTextureColor, vertexColor, u_textureColorLerpFactor);
    }
    else
    {
        resultColor = vertexColor;
    }

    FragColor = vec4(resultColor, 1.0);
}
