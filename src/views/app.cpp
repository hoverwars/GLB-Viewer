#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "app.h"

App::App(GLFWwindow* window) : _window(window)
{ }

void App::render()
{
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _renderHeaderMenu();
}

void App::_renderHeaderMenu()
{
    ImGui::BeginMainMenuBar();
    
    if (ImGui::BeginMenu("Menu")) 
    {
        ImGui::MenuItem("Open File", "Ctrl+N", false, false);

        ImGui::Separator();
        ImGui::MenuItem("Close application", NULL, &_onQuitClicked);

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    // Menu actions 
    if (this->_onQuitClicked) {
        glfwSetWindowShouldClose(_window, true);
    }
}