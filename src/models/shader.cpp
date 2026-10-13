#include <fstream>
#include <glad/glad.h>
#include <format>

#include "shader.h"
#include "../utils/logs.h"

using namespace std;

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    ifstream vertexFile = ifstream(vertexShaderPath);
    string vertexShaderContent = "";
    string buffer = "";
    if (!vertexFile.is_open())
    {
        logError(format("Cannot open vertex shader file located at {}", vertexShaderPath));
    }
    else
    {
        while (getline(vertexFile, buffer))
        { 
            vertexShaderContent += buffer;
            vertexShaderContent += "\n";
        }
    }

    ifstream fragmentFile = ifstream(fragmentShaderPath);
    string fragmentShaderContent = "";
    if (!fragmentFile.is_open())
    {
        logError(format("Cannot open fragment shader file located at {}", fragmentShaderPath));
    }
    else
    {
        while (getline(fragmentFile, buffer))
        { 
            fragmentShaderContent += buffer;
            fragmentShaderContent += "\n";
        }
    }

    unsigned int vertexShaderId;
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    const char* charVertexShaderContent = vertexShaderContent.c_str();
    glShaderSource(vertexShaderId, 1, &charVertexShaderContent, NULL);
    glCompileShader(vertexShaderId);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        logError("Error while compiling vertex shader.");
        logError(infoLog);
    }

    unsigned int fragmentShaderId;
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    const char* charFragmentShaderContent = fragmentShaderContent.c_str();
    glShaderSource(fragmentShaderId, 1, &charFragmentShaderContent, NULL);
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
        logError("Error while compiling fragment shader.");
        logError(infoLog);
    }

    // Shader program
    _shaderProgram = glCreateProgram();

    glAttachShader(_shaderProgram, vertexShaderId);
    glAttachShader(_shaderProgram, fragmentShaderId);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        logError("Error while linking shader program.");
        logError(infoLog);
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    logInfo("Initialized shader programs");
    logInfo("Succcesfully setup scene rendering");
}

void Shader::use()
{
    glUseProgram(_shaderProgram);
}

int Shader::getUniformLocation(const char* variable)
{
    int value = glGetUniformLocation(_shaderProgram, variable);
    if (value == - 1)
    {
        logWarning(format("Variable '{}' was not found in this shader.", variable));
    }

    return value;
}