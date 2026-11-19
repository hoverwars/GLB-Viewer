#ifndef APP_CONTEXT_PROVIDER_H
#define APP_CONTEXT_PROVIDER_H

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

#include "../models/position.h"

class WindowContextProvider
{
private:
    static WindowContextProvider* _instance;
    static std::vector<std::function<void(GLFWwindow*, double, double)>> _callbacks;
    static bool _isListeningToScroll;

    GLFWwindow* _window;
public:
    WindowContextProvider(GLFWwindow* window);
    static WindowContextProvider* getInstance();
    static void setInstance(WindowContextProvider* instance);
    Size2D getFramebufferSize();

    void shouldClose(bool value);

    // Inputs
    void addScrollCallback(std::function<void(GLFWwindow*, double, double)> callback);
};

#endif