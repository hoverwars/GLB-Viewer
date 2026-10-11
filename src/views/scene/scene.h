#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>

#include "../../models/position.h"

class Scene
{
private:
    GLFWwindow* _window;
    unsigned int _shaderProgram;
    unsigned int _vbo; // Vertex buffer object
    unsigned int _vao; // Vertex array object
    unsigned int _ebo; // Element buffer object
    void renderContent();
    void initShaderProgram();

public:
    Scene(GLFWwindow* window);
    void render(Position scenePos);

};

#endif