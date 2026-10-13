#ifndef SHADER_H
#define SHADER_H

class Shader
{
private:
    unsigned int _shaderProgram;

public:
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    void use();
    int getUniformLocation(const char* variable);
};

#endif