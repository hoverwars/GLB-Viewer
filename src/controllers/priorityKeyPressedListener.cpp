#include "priorityKeyPressedListener.h"

#include <GLFW/glfw3.h>

#include "windowContextProvider.h"

using namespace std;

PriorityKeyPressedListener::PriorityKeyPressedListener(vector<int> listenedKeys)
{
    _listenedKeys = listenedKeys;
    _priorityQueue = {};
    _callbackRef = WindowContextProvider::getInstance()->addKeyCallback([this](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        keyListener(key, action);
    });
}

PriorityKeyPressedListener::~PriorityKeyPressedListener()
{
    WindowContextProvider::getInstance()->removeKeyCallback(_callbackRef);
}

// public methods

int PriorityKeyPressedListener::lastPressed()
{
    return _priorityQueue.empty() ? -1 : _priorityQueue.back();
}

// Private methods

deque<int>::iterator PriorityKeyPressedListener::indexOf(int key)
{
    deque<int>::iterator index = _priorityQueue.end();
    for (deque<int>::iterator it = _priorityQueue.begin(); it != _priorityQueue.end(); ++it) 
    {
        if (*it == key)
        {
            index = it;
            break;
        }
    }

    return index;
}

bool PriorityKeyPressedListener::removeIfPresent(int key)
{
    deque<int>::iterator index = indexOf(key);
    
    if (index != _priorityQueue.end())
    {
        _priorityQueue.erase(index);
    }
    
    return index != _priorityQueue.end();
}

bool PriorityKeyPressedListener::insertIfAbsent(int key)
{
    deque<int>::iterator index = indexOf(key);

    if (index == _priorityQueue.end())
    {
        _priorityQueue.push_back(key);
    }

    return index == _priorityQueue.end();
}

void PriorityKeyPressedListener::keyListener(int key, int action)
{
    for (int listenedKey : _listenedKeys)
    {
        if (listenedKey == key)
        {
            switch (action)
            {
                case GLFW_PRESS:
                    insertIfAbsent(key);
                    break;

                case GLFW_RELEASE:
                    removeIfPresent(key);
                    break;
            }
        }
    }
}