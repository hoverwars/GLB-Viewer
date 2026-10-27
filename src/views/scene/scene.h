#ifndef SCENE_H
#define SCENE_H

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

#include "../../models/position.h"
#include "../../models/shader.h"
#include "../../models/framebuffer.h"
#include "../../models/glb/node.h"

using namespace std;

class Scene
{
private:
    Shader* _shader;
    Framebuffer* _framebuffer;
    function<void()> _renderer;

    vector<glm::vec3> _grid;
    vector<glm::vec3> _subGrid;

    static double _cameraZoom;
    static bool _isSceneHovered;

    unsigned int _vbo; // Vertex buffer object
    unsigned int _vao; // Vertex array object
    unsigned int _ebo; // Element buffer object
    void renderContent();
    static void updateZoom(GLFWwindow* window, double xOffset, double yOffset);

public:
    Scene();
    ~Scene();
    void render(Region2D scenePos);
    void renderNode(Node* node);
    void renderGrid();
};

#endif