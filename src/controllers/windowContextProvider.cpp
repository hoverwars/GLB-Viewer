#include "windowContextProvider.h"

using namespace std;

WindowContextProvider* WindowContextProvider::_instance = nullptr;
scrollCallbackContainer WindowContextProvider::_scrollCallbacks = {};
keyCallbackContainer WindowContextProvider::_keyCallbacks = {};

bool WindowContextProvider::_isListeningToScroll = false;
bool WindowContextProvider::_isListeningToKey = false;

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

scrollCallbackContainer::iterator WindowContextProvider::addScrollCallback(function<void(GLFWwindow*, double, double)> callback)
{
    // Enable scroll listening for the next frame.
    if (!_isListeningToScroll)
    {
        _isListeningToScroll = true;

        auto scrollCallback = [](GLFWwindow* window, double xOffset, double yOffset)
        {
            for (auto callback : _scrollCallbacks)
            {
                callback(window, xOffset, yOffset);
            }
        };

        glfwSetScrollCallback(_window, scrollCallback);
    }

    _scrollCallbacks.push_back(callback);
    return --_scrollCallbacks.end();
}

keyCallbackContainer::iterator WindowContextProvider::addKeyCallback(function<void(GLFWwindow*, int, int, int, int)> callback)
{
    // Enable scroll listening for the next frame.
    if (!_isListeningToKey)
    {
        _isListeningToKey = true;

        auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            for (auto callback : _keyCallbacks)
            {
                callback(window, key, scancode, action, mods);
            }
        };

        glfwSetKeyCallback(_window, keyCallback);
    }

    _keyCallbacks.push_back(callback);
    return --_keyCallbacks.end();
}

void WindowContextProvider::removeKeyCallback(keyCallbackContainer::iterator it)
{
    _keyCallbacks.erase(it);
}