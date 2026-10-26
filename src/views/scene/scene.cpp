#include <format>
#include <string>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"
#include "imgui/imgui.h"
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

    glLineWidth(2);
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
}

void Scene::render(Region2D scenePos)
{
    Size2D fbSize = WindowContextProvider::getInstance()->getFramebufferSize();

    int windowHeight = fbSize.height - 30;
    
    ImGui::SetNextWindowPos(ImVec2(scenePos.x, 25));
    ImGui::SetNextWindowSize(ImVec2(scenePos.width, windowHeight));
    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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

    ImGui::End();
}

void Scene::renderContent()
{
    glClearColor(tertiaryBgColor.x, tertiaryBgColor.y, tertiaryBgColor.z, tertiaryBgColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader->use();

    // MVP
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
    glUniformMatrix4fv(_shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Camera
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 50.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(_shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    ImVec2 spaceAvailable = ImGui::GetContentRegionAvail();
    projection = glm::perspective(glm::radians(45.0f), spaceAvailable.x / spaceAvailable.y, 0.1f, 1000.0f);
    glUniformMatrix4fv(_shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    renderGrid();

    for (Node* node: NodeHandler::getInstance()->getNodes())
    {
        renderNode(node);
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

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glUniform3f(_shader->getUniformLocation("inputColor"), shaderLineColor.x, shaderLineColor.y, shaderLineColor.z);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
            glBindVertexArray(0);
        }
    }
}