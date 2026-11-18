#include "leftMenu.h"

#include <imgui/imgui.h>
#include <imgui/IconsLucide.h>
#include <format>
#include <glm/gtc/quaternion.hpp>

#include "../../controllers/windowContextProvider.h"
#include "../../controllers/nodeHandler.h"
#include "../../utils/darkStyle.h"

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

    renderTransformInspector();

    ImGui::End();

    return { 5, 25, static_cast<int>(size.x), static_cast<int>(size.y) };
}

// Numeric edition of the first selected node (local space, glTF axes: Y-up).
void LeftMenu::renderTransformInspector()
{
    vector<Node*> selected = NodeHandler::getInstance()->getSelectedRoots();
    if (selected.empty()) return;

    Node* node = selected[0];
    ImGui::Separator();
    ImGui::Text(ICON_LC_MOVE_3D " Transform: %s", node->getName().c_str());

    glm::vec3 translation = node->getTranslation();
    if (ImGui::DragFloat3("Position", &translation.x, 0.05f))
        node->setTranslation(translation);

    glm::vec4 r = node->getRotation();
    glm::vec3 euler = glm::degrees(glm::eulerAngles(glm::quat(r.w, r.x, r.y, r.z)));
    if (ImGui::DragFloat3("Rotation", &euler.x, 0.5f))
    {
        glm::quat q = glm::quat(glm::radians(euler));
        node->setRotation(glm::vec4(q.x, q.y, q.z, q.w));
    }

    glm::vec3 scale = node->getScale();
    if (ImGui::DragFloat3("Scale", &scale.x, 0.01f))
        node->setScale(scale);

    if (selected.size() > 1)
        ImGui::TextDisabled("(+%d other selected)", (int)selected.size() - 1);
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
        if (node->isSelected())
            ImGui::TextColored(primaryColor, node->getName().c_str());
        else
            ImGui::Text(node->getName().c_str());

        if (ImGui::IsItemClicked())
        {
            node->setSelected(!node->isSelected());
        }

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
        if (node->isSelected())
            ImGui::TextColored(primaryColor, node->getName().c_str());
        else
            ImGui::Text(node->getName().c_str());

        if (ImGui::IsItemClicked())
        {
            node->setSelected(!node->isSelected());
        }
    }
}
