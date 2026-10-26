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

    for (Node* node: NodeHandler::getInstance()->getNodes())
    {
        renderNode(node);
    }

    //float timeValue = glfwGetTime();
    //float loc = (sin(timeValue) / 3.0f);
    /*float vertices[] = {
        // bottom face
        0.0f, 0.66f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.66f, 1.0f, 1.0f, 1.0f
    };  

    // From the array of vertices + colors, build some triangles.
    unsigned int indices[] = { 
        0, 1, 2,
        0, 1, 3,
        1, 2, 3,
        2, 0, 3
    };
    
    // Bind our buffers
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    
    // Specifying to OpenGL how to interpret our vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * (sizeof(float))));
    glEnableVertexAttribArray(1);
    
    // MVP
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
    glUniformMatrix4fv(_shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Camera
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(_shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection;
    // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    ImVec2 spaceAvailable = ImGui::GetContentRegionAvail();
    projection = glm::perspective(glm::radians(45.0f), spaceAvailable.x / spaceAvailable.y, 0.1f, 100.0f);
    glUniformMatrix4fv(_shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);*/
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
            
            // MVP
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
            glUniformMatrix4fv(_shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
            
            // Camera
            glm::vec3 cameraPos   = glm::vec3(-75.0f, 0.0f, 300.0f);
            glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glUniformMatrix4fv(_shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));

            glm::mat4 projection;
            // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
            ImVec2 spaceAvailable = ImGui::GetContentRegionAvail();
            projection = glm::perspective(glm::radians(45.0f), spaceAvailable.x / spaceAvailable.y, 0.1f, 1000.0f);
            glUniformMatrix4fv(_shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

            
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