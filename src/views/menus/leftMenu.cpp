#include "imgui/imgui.h"
#include "leftMenu.h"

#include "../../controllers/windowContextProvider.h"
#include "../../controllers/nodeHandler.h"

Region2D LeftMenu::render()
{
    Size2D fbSize = WindowContextProvider::getInstance()->getFramebufferSize();

    int windowHeight = fbSize.height - 30;
    ImGui::SetNextWindowPos(ImVec2(5, 25));
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(200, windowHeight),
        ImVec2(600, windowHeight)
    );

    ImGui::Begin("Objects", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImVec2 size = ImGui::GetWindowSize();

    for (Node* node : NodeHandler::getInstance()->getNodes())
    {
        renderNode(node);
    }

    ImGui::End();

    return { 5, 25, static_cast<int>(size.x), static_cast<int>(size.y) };
}

void LeftMenu::renderNode(Node* node)
{
    if (node->hasChild() && ImGui::TreeNode(node->getName().c_str()))
    {
        for (Node* child : node->getChildren())
        {
            renderNode(child);
        }

        ImGui::TreePop();
    }
    else if (!node->hasChild())
    {
        ImGui::Text(node->getName().c_str());
    }
}