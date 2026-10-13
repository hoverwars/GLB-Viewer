#ifndef APP_H
#define APP_H

#include <GLFW/glfw3.h>
#include "menus/leftMenu.h"
#include "scene/scene.h"

class App
{
private:
    LeftMenu* _leftMenu;
    Scene* _scene;
    bool _onQuitClicked = false;

    void _renderHeaderMenu();
public:
    App();
    ~App();
    void render();
};

#endif