#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "imgui/imgui.h"
#include "../utils/darkStyle.h"
#include "../models/position.h"

App::App(GLFWwindow* window) : _window(window), _leftMenu(new LeftMenu(window)), _scene(new Scene(window))
{ 
    ImGui::PushStyleColor(ImGuiCol_WindowBg, primaryBgColor);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, primaryBgColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, secondaryBgColor);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, secondaryBgColor);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, primaryColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, primaryColor);
}

App::~App()
{
    delete _leftMenu;
}

void App::render()
{
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);

    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //ImGui::ShowStyleEditor();

    _renderHeaderMenu();
    Position pos = _leftMenu->render();
    _scene->render({
        pos.x + pos.width + 5,
        5,  // Use openGL coordinate, where 0, 0 is bottom left, not up right.
        width - pos.x - pos.width - 10,
        height - 30
    });
}

void App::_renderHeaderMenu()
{
    bool result = ImGui::BeginMainMenuBar();
    if (!result) return;

    if (ImGui::BeginMenu("Menu")) 
    {
        ImGui::MenuItem("Open File", NULL, false, false);

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