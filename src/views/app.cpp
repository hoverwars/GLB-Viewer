#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "menus/leftMenu.h"
#include "../utils/darkStyle.h"
#include "app.h"

App::App(GLFWwindow* window) : _window(window), _leftMenu(new LeftMenu(window))
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
    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //ImGui::ShowStyleEditor();

    _renderHeaderMenu();
    _leftMenu->render();
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