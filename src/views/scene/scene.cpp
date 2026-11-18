#include <format>
#include <string>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui/imgui.h>
#include <imgui/IconsLucide.h>

#include "scene.h"
#include "../../utils/darkStyle.h"
#include "../../utils/logs.h"
#include "../../controllers/windowContextProvider.h"
#include "../../controllers/nodeHandler.h"

using namespace std;

Scene::Scene()
{
    logInfo("Starting scene setup...");
    _shader = new Shader(
        "resource/shaders/default_vertex_shader.vert", 
        "resource/shaders/default_fragment_shader.frag"
    );
    _framebuffer = new Framebuffer();
    _renderer = [this]() { this->renderContent(); };
    _camera = new Camera();

    glLineWidth(3);
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    glEnable(GL_DEPTH_TEST);

    _grid = vector<glm::vec3>();
    for (float i = -100; i <= 100; i += 1)
    {
        _grid.push_back(glm::vec3(-100.0f, (float)i, 0.0f));
        _grid.push_back(glm::vec3(100.0f, (float)i, 0.0f));

        _grid.push_back(glm::vec3((float)i, -100.0f, 0.0f));
        _grid.push_back(glm::vec3((float)i, 100.0f, 0.0f));
    }

    _subGrid = vector<glm::vec3>();
    for (float i = -10; i <= 10; i += 1)
    {
        _subGrid.push_back(glm::vec3(-100.0f, (float)i*10, 0.0f));
        _subGrid.push_back(glm::vec3(100.0f, (float)i*10, 0.0f));

        _subGrid.push_back(glm::vec3((float)i*10, -100.0f, 0.0f));
        _subGrid.push_back(glm::vec3((float)i*10, 100.0f, 0.0f));
    }

    logInfo("Buffers ready.");
}

Scene::~Scene()
{
    delete _shader;
    delete _framebuffer;
    delete _camera;
}

void Scene::render(Region2D scenePos)
{
    Size2D fbSize = WindowContextProvider::getInstance()->getFramebufferSize();

    int windowHeight = fbSize.height - 30;
    
    ImGui::SetNextWindowPos(ImVec2(scenePos.x, 25));
    ImGui::SetNextWindowSize(ImVec2(scenePos.width, windowHeight));

    if (ImGui::Begin(ICON_LC_BOXES " Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        renderMenu();
        
        ImVec2 spaceAvailablePos = ImGui::GetCursorScreenPos();
        ImVec2 spaceAvailable = ImGui::GetContentRegionAvail();
        
        _framebuffer->renderToFramebuffer(
            spaceAvailable.x,
            spaceAvailable.y,
            _renderer
        );
    
        ImGui::GetWindowDrawList()->AddImage(
            _framebuffer->getTextureId(), 
            spaceAvailablePos, 
            ImVec2(spaceAvailablePos.x + spaceAvailable.x, spaceAvailablePos.y + spaceAvailable.y), 
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );
    
        _camera->setSceneHovered(ImGui::IsWindowHovered());
        handleTransformInput(spaceAvailablePos, spaceAvailable);
    
        ImGui::End();
    }
}

void Scene::renderContent()
{
    glClearColor(tertiaryBgColor.x, tertiaryBgColor.y, tertiaryBgColor.z, tertiaryBgColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader->use();

    // MVP
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(_shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Camera (new)
    ImVec2 spaceAvailable = ImGui::GetContentRegionAvail();
    glm::mat4 view = _camera->updateCamera(spaceAvailable.y);

    glUniformMatrix4fv(_shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    projection = glm::perspective(glm::radians(_camera->K_FOV_DEGREES), spaceAvailable.x / spaceAvailable.y, 0.1f, 1000.0f);
    glUniformMatrix4fv(_shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    if (_showGrid)
        renderGrid();

    for (Node* node: NodeHandler::getInstance()->getNodes())
    {
        renderNode(node);
    }
}

// GLTF is Y-up but the application is Z up.
static glm::mat4 yUpToZUp()
{
    return glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

// Left mouse drag in the scene transforms the selected nodes, depending on the mouse mode:
//  - move (1): drag in the camera plane, Shift = vertical (Z) only
//  - scale (2): drag up / down to grow / shrink uniformly
//  - rotate (3): drag left / right = around Z, up / down = around the screen horizontal axis
// In every mode, holding X / Y / Z constrains the action to that world axis.
void Scene::handleTransformInput(ImVec2 areaPos, ImVec2 areaSize)
{
    ImGuiIO& io = ImGui::GetIO();

    // A drag starts only on the scene view (not on its toolbar) and continues when the cursor leaves it.
    bool overView = io.MousePos.x >= areaPos.x && io.MousePos.x <= areaPos.x + areaSize.x &&
                    io.MousePos.y >= areaPos.y && io.MousePos.y <= areaPos.y + areaSize.y;
    if (_mouseMode != 0 && overView && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        _isTransforming = true;
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left) || _mouseMode == 0)
        _isTransforming = false;

    if (!_isTransforming || (io.MouseDelta.x == 0 && io.MouseDelta.y == 0))
        return;

    // Constraint axis (world space), chosen by holding X / Y / Z.
    bool constrained = true;
    glm::vec3 constraint(0.0f);
    if (ImGui::IsKeyDown(ImGuiKey_X)) constraint = glm::vec3(1.0f, 0.0f, 0.0f);
    else if (ImGui::IsKeyDown(ImGuiKey_Y)) constraint = glm::vec3(0.0f, 1.0f, 0.0f);
    else if (ImGui::IsKeyDown(ImGuiKey_Z)) constraint = glm::vec3(0.0f, 0.0f, 1.0f);
    else constrained = false;

    for (Node* node : NodeHandler::getInstance()->getSelectedRoots())
    {
        // World -> local space of the node's father, to express world-space deltas in the node's own space.
        glm::mat4 parentWorld = node->getFather() == nullptr ? yUpToZUp() : node->getFather()->applyTransformations(yUpToZUp());
        glm::mat3 worldToParent = glm::inverse(glm::mat3(parentWorld));

        if (_mouseMode == 1)
        {
            // Scale by the depth of the object itself, so that it follows the cursor.
            glm::vec3 worldPos = glm::vec3(parentWorld * glm::vec4(node->getTranslation(), 1.0f));
            float unitsPerPixel = _camera->getUnitsPerPixelAt(worldPos, areaSize.y);
            glm::vec3 worldDelta;
            if (constrained)
            {
                // Direction of the axis on screen (pixels, y down); the object slides along it, following the cursor.
                glm::vec2 screenAxis(glm::dot(constraint, _camera->getRight()), -glm::dot(constraint, _camera->getUp()));
                float lengthSq = glm::dot(screenAxis, screenAxis);
                if (lengthSq < 0.01f) continue;  // Axis pointing at the camera: no meaningful drag direction.
                float amount = (io.MouseDelta.x * screenAxis.x + io.MouseDelta.y * screenAxis.y) / lengthSq;
                worldDelta = constraint * amount * unitsPerPixel;
            }
            else if (io.KeyShift)
                worldDelta = glm::vec3(0.0f, 0.0f, -io.MouseDelta.y) * unitsPerPixel;
            else
                worldDelta = (_camera->getRight() * io.MouseDelta.x - _camera->getUp() * io.MouseDelta.y) * unitsPerPixel;

            node->setTranslation(node->getTranslation() + worldToParent * worldDelta);
        }
        else if (_mouseMode == 2)
        {
            float factor = glm::clamp(exp(-io.MouseDelta.y * 0.01f), 0.5f, 2.0f);
            glm::vec3 scaleFactor(factor);
            if (constrained)
            {
                // Scale lives in the node's own axes: find which one is closest to the world axis.
                glm::vec4 r = node->getRotation();
                glm::vec3 local = glm::inverse(glm::quat(r.w, r.x, r.y, r.z)) * (worldToParent * constraint);
                glm::vec3 absLocal = glm::abs(local);
                int axisIndex = absLocal.x >= absLocal.y && absLocal.x >= absLocal.z ? 0 : (absLocal.y >= absLocal.z ? 1 : 2);
                scaleFactor = glm::vec3(1.0f);
                scaleFactor[axisIndex] = factor;
            }
            node->setScale(node->getScale() * scaleFactor);
        }
        else if (_mouseMode == 3)
        {
            // Constrained: turn around the held axis (drag right = positive).
            // Otherwise: horizontal drag turns around Z, vertical drag around the camera's horizontal axis.
            glm::vec3 worldDelta(0.0f);  // Rotation vector in world space (axis * angle in degrees)
            float horizontal = io.MouseDelta.x * 0.5f;
            float vertical = io.MouseDelta.y * 0.5f;
            if (constrained) worldDelta = constraint * horizontal;
            else worldDelta = glm::vec3(0.0f, 0.0f, 1.0f) * horizontal + _camera->getRight() * vertical;

            float angle = glm::length(worldDelta);
            if (angle == 0.0f) continue;

            glm::vec3 axis = glm::normalize(worldToParent * (worldDelta / angle));
            glm::quat delta = glm::angleAxis(glm::radians(angle), axis);
            glm::vec4 r = node->getRotation();
            glm::quat result = glm::normalize(delta * glm::quat(r.w, r.x, r.y, r.z));
            node->setRotation(glm::vec4(result.x, result.y, result.z, result.w));
        }
    }
}

void Scene::renderGrid()
{
    // Draw grid
    glLineWidth(1);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _grid.size() * sizeof(glm::vec3), _grid.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUniform3f(_shader->getUniformLocation("inputColor"), shaderLineColor.x, shaderLineColor.y, shaderLineColor.z);
    glDrawArrays(GL_LINES, 0, _grid.size());
    glBindVertexArray(0);

    // Draw subgrid 
    glLineWidth(3);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _subGrid.size() * sizeof(glm::vec3), _subGrid.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUniform3f(_shader->getUniformLocation("inputColor"), shaderLineColor.x, shaderLineColor.y, shaderLineColor.z);
    glDrawArrays(GL_LINES, 0, _subGrid.size());
    glBindVertexArray(0);

    // Reset line width
    glLineWidth(1);
}

void Scene::renderNode(Node* node)
{   
    // Early return if this node should not be displayed 
    // (a node not visible with children doesnt) display its children
    if (!node->isVisible()) return;

    // If this node has children, display them.
    if (node->hasChild())
    {
        for (Node* child : node->getChildren())
        {
            renderNode(child);
        }
    }

    // If this node has a mesh, display this node.
    Mesh* mesh = node->getMesh();
    if (mesh != nullptr)
    {
        // GLTF is Y-up but the application is Z up. Apply a rotation to transform the model to be z-up 
        glm::mat4 model = node->applyTransformations(yUpToZUp());
        glUniformMatrix4fv(_shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

        for (MeshPart* part : mesh->getParts())
        {
            vector<glm::vec3> vertices = part->getVertices()->getData();
            vector<unsigned short> indices = part->getIndices()->getData();

            // Bind our buffers
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(short), indices.data(), GL_STATIC_DRAW); 
            
            // Specifying to OpenGL how to interpret our vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
            glEnableVertexAttribArray(0);  
                        
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glUniform3f(_shader->getUniformLocation("inputColor"), shaderFillColor.x, shaderFillColor.y, shaderFillColor.z);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
            
            if (_displayLines)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                ImVec4 lineColor = node->isSelected() ? shaderSelectedLineColor : shaderLineColor;
                glUniform3f(_shader->getUniformLocation("inputColor"), lineColor.x, lineColor.y, lineColor.z);
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
            }

            glBindVertexArray(0);
        }
    }
}

bool displayButton(string icon, bool selected, bool applySameLineAttribute = true)
{
    if (selected)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, primaryColor);
    }

    if (applySameLineAttribute)
        ImGui::SameLine();
    
    bool clicked = ImGui::Button(icon.c_str());

    if (selected)
    {
        ImGui::PopStyleColor(1);
    }

    return clicked;
}

void Scene::renderMenu()
{
    ImGui::PushFont(NULL, ImGui::GetStyle().FontSizeBase * 2);

    bool wasTargetingCenter = _camera->isTargetingCenter(); // Pop style after.
    if (wasTargetingCenter)
        ImGui::PushStyleColor(ImGuiCol_Text, primaryColor);

    if (ImGui::Button(ICON_LC_MAP_PINNED))
    {
        _camera->resetPosition();
    }

    if (wasTargetingCenter)
        ImGui::PopStyleColor(1);

    ImGui::SameLine();
    bool oldShowGrid = _showGrid; // To pop style.
    if (oldShowGrid)
        ImGui::PushStyleColor(ImGuiCol_Text, primaryColor);

    if (ImGui::Button(ICON_LC_GRID_3_X_3))
    {
        _showGrid = !_showGrid;
    }

    if (oldShowGrid)
        ImGui::PopStyleColor(1);

    bool oldDisplayLine = _displayLines; // To pop after
    if (oldDisplayLine)
        ImGui::PushStyleColor(ImGuiCol_Text, primaryColor);

    ImGui::SameLine();
    if (ImGui::Button(ICON_LC_SPLINE))
    {
        _displayLines = !_displayLines;
    }

    if (oldDisplayLine)
        ImGui::PopStyleColor(1);

    ImGui::SameLine(0, 50);

    if (displayButton(ICON_LC_MOUSE_POINTER, _mouseMode == 0, false))
        _mouseMode = 0;

    if (displayButton(ICON_LC_MOVE_3D, _mouseMode == 1))
        _mouseMode = 1;

    if (displayButton(ICON_LC_SCALING, _mouseMode == 2))
        _mouseMode = 2;

    if (displayButton(ICON_LC_ROTATE_3D, _mouseMode == 3))
        _mouseMode = 3;

    ImGui::PopFont();
}