#include "leftMenu.h"

#include <imgui/imgui.h>
#include <imgui/IconsLucide.h>
#include <format>

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

    ImGui::Begin(ICON_LC_COMBINE " Objects", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
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
    if (node->hasChild())
    {
        bool opened = ImGui::TreeNode(std::format("##{}", node->getUniqueId()).c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton(std::format("{}##{}", (node->isVisible() ? ICON_LC_EYE_OFF : ICON_LC_EYE), node->getUniqueId()).c_str()))
            node->switchVisibility();
        
        ImGui::SameLine();
        ImGui::Text(node->getName().c_str());

        if (opened)
        {
            for (Node* child : node->getChildren())
            {
                renderNode(child);
            }

            ImGui::TreePop();
        }
        
    }
    
    if (!node->hasChild())
    {
        if (node->isFatherVisible())
        {
            if (ImGui::SmallButton(std::format("{}##{}", (node->isVisible() ? ICON_LC_EYE_OFF : ICON_LC_EYE), node->getUniqueId()).c_str()))
                node->switchVisibility();
        }
        else
        {
            ImGui::SmallButton(std::format("{}##{}", ICON_LC_BAN, node->getUniqueId()).c_str());
        }


        ImGui::SameLine();
        ImGui::Text(node->getName().c_str());
    }
}
