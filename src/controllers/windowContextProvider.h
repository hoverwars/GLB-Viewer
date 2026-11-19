#ifndef APP_CONTEXT_PROVIDER_H
#define APP_CONTEXT_PROVIDER_H

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

#include "../models/position.h"

using keyCallbackContainer = std::vector<std::function<void(GLFWwindow*, int, int, int, int)>>;
using scrollCallbackContainer = std::vector<std::function<void(GLFWwindow*, double, double)>>;

class WindowContextProvider
{
private:
    static WindowContextProvider* _instance;
    static scrollCallbackContainer _scrollCallbacks;
    static keyCallbackContainer _keyCallbacks;
    static bool _isListeningToScroll;
    static bool _isListeningToKey;

    GLFWwindow* _window;
public:
    WindowContextProvider(GLFWwindow* window);
    static WindowContextProvider* getInstance();
    static void setInstance(WindowContextProvider* instance);
    Size2D getFramebufferSize();

    void shouldClose(bool value);

    // Inputs
    scrollCallbackContainer::iterator addScrollCallback(std::function<void(GLFWwindow*, double, double)> callback);
    keyCallbackContainer::iterator addKeyCallback(std::function<void(GLFWwindow*, int, int, int, int)> callback);
    void removeKeyCallback(keyCallbackContainer::iterator it);
};

#endif