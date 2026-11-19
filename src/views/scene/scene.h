#ifndef SCENE_H
#define SCENE_H

#include <functional>
#include <vector>

#include "../../models/position.h"
#include "../../models/shader.h"
#include "../../models/framebuffer.h"
#include "../../models/glb/node.h"
#include "../../models/camera.h"
#include "../../controllers/priorityKeyPressedListener.h"

using namespace std;

#define FREE_MOUSE 0
#define TRANSLATE 1
#define SCALE 2
#define ROTATE 3

class Scene
{
private:
    Shader* _shader;
    Framebuffer* _framebuffer;
    Camera* _camera;
    PriorityKeyPressedListener* _keyListener;

    function<void()> _renderer;

    vector<glm::vec3> _grid;
    vector<glm::vec3> _subGrid;

    // apply transformations to selected node if in transformation mode and transforming object.
    bool _isTransforming;
    void handleTransformations();

    // Menu options
    bool _showGrid = true;
    bool _displayLines = true;
    short _mouseMode = 0;

    unsigned int _vbo; // Vertex buffer object
    unsigned int _vao; // Vertex array object
    unsigned int _ebo; // Element buffer object
    void renderContent();
    void renderNode(Node* node);
    void renderGrid();
    void renderMenu();
public:
    Scene();
    ~Scene();
    void render(Region2D scenePos);
};

#endif