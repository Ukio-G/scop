#version 330 core

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform bool normalMapUsage;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
  vec3 TangentLightPos;
  vec3 TangentViewPos;
  vec3 TangentFragPos;

  vec3 TangentT;
  vec3 TangentB;
}
fs_in;
out vec4 FragColor;

void main() {
  if (normalMapUsage) {
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0); // this normal is in tangent space

    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    FragColor = vec4(diffuse, 1.0);
  } else {

    // ambient
    float ambientStrength = 0.02;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) *
                  texture(diffuseMap, fs_in.TexCoords).rgb;
    FragColor = vec4(result, 1.0);
  }
}
