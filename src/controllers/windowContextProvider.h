#ifndef APP_CONTEXT_PROVIDER_H
#define APP_CONTEXT_PROVIDER_H

#include <GLFW/glfw3.h>

#include "../models/position.h"

class WindowContextProvider
{
private:
    static WindowContextProvider* _instance;
    GLFWwindow* _window;
public:
    WindowContextProvider(GLFWwindow* window);
    static WindowContextProvider* getInstance();
    static void setInstance(WindowContextProvider* instance);
    Size2D getFramebufferSize();
    void shouldClose(bool value);
};

#endif