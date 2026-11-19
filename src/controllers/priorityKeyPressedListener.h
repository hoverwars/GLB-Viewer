#ifndef PRIORITY_KEY_PRESSED_LISTENER_H
#define PRIORITY_KEY_PRESSED_LISTENER_H

#include <GLFW/glfw3.h>

#include <vector>
#include <deque>
#include <functional>

using namespace std;
using keyCallbackIterator = std::vector<std::function<void(GLFWwindow*, int, int, int, int)>>::iterator;

/*
 * Listen to key pressed by the user. The last key pressed takes the priority.
*/
class PriorityKeyPressedListener
{
private:
    vector<int> _listenedKeys;
    deque<int> _priorityQueue;
    keyCallbackIterator _callbackRef;

    deque<int>::iterator indexOf(int key); // Returns _priority.end() if not found or empty.
    bool removeIfPresent(int key);
    bool insertIfAbsent(int key);

    void keyListener(int key, int action);

public:
    PriorityKeyPressedListener(vector<int> listenedKeys);
    ~PriorityKeyPressedListener();
    int lastPressed(); // Returns -1 if no key is pressed.
};

#endif