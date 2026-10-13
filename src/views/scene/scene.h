#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>

#include "../../models/position.h"
#include "../../models/shader.h"

class Scene
{
private:
    GLFWwindow* _window;
    Shader _shader;
    unsigned int _vbo; // Vertex buffer object
    unsigned int _vao; // Vertex array object
    unsigned int _ebo; // Element buffer object
    void renderContent();

public:
    Scene(GLFWwindow* window);
    void render(Position scenePos);

};

#endif