#ifndef APP_H
#define APP_H

#include <GLFW/glfw3.h>

class App
{
private:
    GLFWwindow* _window;
    bool _onQuitClicked = false;

    void _renderHeaderMenu();
public:
    App(GLFWwindow* window);
    void render();
};

#endif