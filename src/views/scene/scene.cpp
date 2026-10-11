#include <format>
#include <string>
#include <glad/glad.h>

#include "scene.h"
#include "../../utils/darkStyle.h"
#include "../../utils/logs.h"

Scene::Scene(GLFWwindow* window) : _window(window)
{
    logInfo("Starting scene setup...");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    logInfo("Buffers ready.");
    initShaderProgram();
    logInfo("Initialized shader programs");
    logInfo("Succcesfully setup scene rendering");
}

void Scene::render(Position scenePos)
{
    // Postulate: the current viewport takes all the window.
    int oldVpWidth, oldVpHeight;
    glfwGetFramebufferSize(_window, &oldVpWidth, &oldVpHeight);

    glViewport(scenePos.x, scenePos.y, scenePos.width, scenePos.height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(scenePos.x, scenePos.y, scenePos.width, scenePos.height);

    glClearColor(tertiaryBgColor.x, tertiaryBgColor.y, tertiaryBgColor.z, tertiaryBgColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    renderContent();

    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, oldVpWidth, oldVpHeight);
}

void Scene::renderContent()
{
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Bind our buffers
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // Specifying to OpenGL how to interpret our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    
    glUseProgram(_shaderProgram);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Scene::initShaderProgram()
{
    // Load the vertex shader
    const char *vertexShaderSource = "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        logError("Error while compiling vertex shader.");
        logError(infoLog);
    }

    //  Load the fragment shader
    const std::string source = std::format("#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{{\n"
            "FragColor = vec4({:.3f}, {:.3f}, {:.3f}, 1.0);\n"
        "}}", 
        shaderColor.x, shaderColor.y, shaderColor.z
    );
    const char* fragmentShaderSource = source.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        logError("Error while compiling fragment shader.");
        logError(infoLog);
    }

    // Shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        logError("Error while linking shader program.");
        logError(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _shaderProgram = shaderProgram;
}