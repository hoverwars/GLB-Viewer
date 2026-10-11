#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "leftMenu.h"

LeftMenu::LeftMenu(GLFWwindow* window) : _window(window)
{ }

void LeftMenu::render()
{
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);

    int windowHeight = height - 30;
    ImGui::SetNextWindowPos(ImVec2(5, 25));
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(200, windowHeight),
        ImVec2(600, windowHeight)
    );

    ImGui::Begin("Objects", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImGui::End();
}