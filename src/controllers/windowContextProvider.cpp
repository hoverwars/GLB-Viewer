#include "windowContextProvider.h"

WindowContextProvider* WindowContextProvider::_instance = nullptr;

WindowContextProvider::WindowContextProvider(GLFWwindow* window) : _window(window)
{ }

WindowContextProvider* WindowContextProvider::getInstance()
{
    return WindowContextProvider::_instance;
}

void WindowContextProvider::setInstance(WindowContextProvider* instance)
{
    WindowContextProvider::_instance = instance;
}

Size2D WindowContextProvider::getFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);

    return { width, height };
}

void WindowContextProvider::shouldClose(bool value)
{
    glfwSetWindowShouldClose(_window, value);
}

void WindowContextProvider::setScrollCallback(GLFWscrollfun callback)
{
    glfwSetScrollCallback(_window, callback);
}