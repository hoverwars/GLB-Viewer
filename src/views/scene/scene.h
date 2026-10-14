#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>
#include <functional>

#include "../../models/position.h"
#include "../../models/shader.h"
#include "../../models/framebuffer.h"

class Scene
{
private:
    Shader* _shader;
    Framebuffer* _framebuffer;
    std::function<void()> _renderer;

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