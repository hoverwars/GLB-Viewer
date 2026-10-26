#ifndef LEFT_MENU_H
#define LEFT_MENU_H

#include <GLFW/glfw3.h>

#include "../../models/position.h"
#include "../../models/glb/node.h"

class LeftMenu
{
private:
    void renderNode(Node* node);
public:
    Region2D render();
};

#endif