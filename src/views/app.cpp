#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "app.h"
#include "../utils/darkStyle.h"
#include "../utils/logs.h"
#include "../models/position.h"
#include "../controllers/windowContextProvider.h"
#include "../controllers/glbFileHandler.h"
#include "../controllers/nodeHandler.h"
#include "../models/glb/glbFile.h"

App::App() : _leftMenu(new LeftMenu()), _scene(new Scene()), _fileBrowser()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, primaryBgColor);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, primaryBgColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, secondaryBgColor);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, secondaryBgColor);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, primaryColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, primaryColor);
    logInfo("Application style setup.");

    GlbFile* file = GlbFileHandler::getInstance()->loadGlbFile("D:/dev/GLB-Viewer/resource/models/gun.glb");
    NodeHandler::getInstance()->loadNodes(file);
}

App::~App()
{
    delete _leftMenu;
}

void App::render()
{
    Size2D size = WindowContextProvider::getInstance()->getFramebufferSize();

    glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //ImGui::ShowStyleEditor();

    _renderHeaderMenu();
    Region2D pos = _leftMenu->render();
    _scene->render({
        pos.x + pos.width + 5,
        5,  // Use openGL coordinate, where 0, 0 is bottom left, not up right.
        size.width - pos.x - pos.width - 10,
        size.height - 30
    });
}

void App::_renderHeaderMenu()
{
    bool result = ImGui::BeginMainMenuBar();
    if (!result) return;

    bool onOpenFileClicked = false;
    if (ImGui::BeginMenu("Menu")) 
    {
        if (ImGui::MenuItem("Open File", NULL))
            onOpenFileClicked = true;

        ImGui::Separator();
        ImGui::MenuItem("Close application", NULL, &_onQuitClicked);

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    // Menu actions 
    if (this->_onQuitClicked) {
        WindowContextProvider::getInstance()->shouldClose(true);
    }

    if (onOpenFileClicked) {
        ImGui::OpenPopup("Open File");
    }

    // Always show file dialog. This method will return if there is no popup existing with the same name
    if (_fileBrowser.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1080, 720), ".glb"))
    {   
        GlbFile* file = GlbFileHandler::getInstance()->loadGlbFile(_fileBrowser.selected_path);
        NodeHandler::getInstance()->loadNodes(file);
    }
}