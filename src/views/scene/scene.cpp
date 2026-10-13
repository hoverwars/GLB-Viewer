#include <format>
#include <string>
#include <cmath>
#include <glad/glad.h>

#include "scene.h"
#include "../../utils/darkStyle.h"
#include "../../utils/logs.h"

Scene::Scene(GLFWwindow* window) : _window(window), _shader("resource/shaders/default_vertex_shader.vert", "resource/shaders/default_fragment_shader.frag")
{
    logInfo("Starting scene setup...");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    logInfo("Buffers ready.");
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
    float timeValue = glfwGetTime();
    float loc = (sin(timeValue) / 3.0f);
    float vertices[] = {
        loc,  loc, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
        loc, -loc, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -loc, -loc, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
        -loc,  loc, 0.0f, 1.0f, 0.0f, 0.0f   // top left 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * (sizeof(float))));
    glEnableVertexAttribArray(1);
    
    _shader.use();
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}