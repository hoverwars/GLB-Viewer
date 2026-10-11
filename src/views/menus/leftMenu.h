#ifndef LEFT_MENU_H
#define LEFT_MENU_H

#include <GLFW/glfw3.h>

class LeftMenu
{
private:
    GLFWwindow* _window;

public:
    LeftMenu(GLFWwindow* window);
    void render();
};

#endif