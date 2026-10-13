#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>

#include "../../models/position.h"
#include "../../models/shader.h"

class Scene
{
private:
    Shader* _shader;
    unsigned int _vbo; // Vertex buffer object
    unsigned int _vao; // Vertex array object
    unsigned int _ebo; // Element buffer object
    void renderContent();

public:
    Scene();
    ~Scene();
    void render(Region2D scenePos);

};

#endif