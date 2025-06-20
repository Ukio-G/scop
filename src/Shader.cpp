#include "Graphic/Shader.hpp"

#include <cstddef>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void Shader::loadFromFile(const char *path, GLuint shaderType) {
    size_t bytes_to_source = 20480;
    shaderSource = new char[bytes_to_source];

    int fd = open(path, O_RDONLY);

    if (fd < 0)
        throw std::runtime_error("Error with open file: " + std::string(path));
    
    int readed = read(fd, shaderSource, bytes_to_source);
    shaderSource[readed] = 0;

    close(fd);

    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    // Compilation log on error
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "Shader compilation failed! Shader type: " 
        << ((shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << "\n"  << infoLog << std::endl;
    }
    
}

ShaderProgram::ShaderProgram(Shader *vertex, Shader *fragment) {
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertex->shaderID);
    glAttachShader(shaderProgram, fragment->shaderID);

    glLinkProgram(shaderProgram);

    // Linkage log on error
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader linkage failed!\n"  << infoLog << std::endl;
    }
}

void ShaderProgram::setBool(const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
}

void ShaderProgram::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
}

void ShaderProgram::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
} 

void ShaderProgram::setFloat4d(const std::string &name, const float (&value)[4] ) const { 
    glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2], value[3]); 
} 

void ShaderProgram::setFloat3d(const std::string &name, const glm42::vec3 & value ) const {
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2]); 
} 

void ShaderProgram::setFloat3d(const std::string &name, const float (&value)[3] ) const { 
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2]); 
} 

void ShaderProgram::setMatrix4d(const std::string &name, const glm42::mat4 & mat) const {
    unsigned int matrixID = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, (const float*)mat.data);
}
