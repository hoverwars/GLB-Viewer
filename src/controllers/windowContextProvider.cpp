#include "windowContextProvider.h"

using namespace std;

WindowContextProvider* WindowContextProvider::_instance = nullptr;
vector<function<void(GLFWwindow*, double, double)>> WindowContextProvider::_callbacks = {};
bool WindowContextProvider::_isListeningToScroll = false;

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

void WindowContextProvider::addScrollCallback(function<void(GLFWwindow*, double, double)> callback)
{
    // Enable scroll listening for the next frame.
    if (!_isListeningToScroll)
    {
        _isListeningToScroll = true;

        auto scrollCallback = [](GLFWwindow* window, double xOffset, double yOffset)
        {
            for (auto callback : WindowContextProvider::_callbacks)
            {
                callback(window, xOffset, yOffset);
            }
        };

        glfwSetScrollCallback(_window, scrollCallback);
    }

    _callbacks.push_back(callback);
}