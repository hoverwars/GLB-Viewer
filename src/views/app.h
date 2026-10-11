#ifndef APP_H
#define APP_H

#include <GLFW/glfw3.h>
#include "menus/leftMenu.h"

class App
{
private:
    GLFWwindow* _window;
    LeftMenu* _leftMenu;
    bool _onQuitClicked = false;

    void _renderHeaderMenu();
public:
    App(GLFWwindow* window);
    ~App();
    void render();
};

#endif