#include <format>
#include <string>
#include <cmath>
#include <glad/glad.h>

#include "scene.h"
#include "imgui/imgui.h"
#include "../../utils/darkStyle.h"
#include "../../utils/logs.h"
#include "../../controllers/windowContextProvider.h"

Scene::Scene()
{
    logInfo("Starting scene setup...");
    _shader = new Shader(
        "resource/shaders/default_vertex_shader.vert", 
        "resource/shaders/default_fragment_shader.frag"
    );
    _framebuffer = new Framebuffer();
    _renderer = [this]() { this->renderContent(); };

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
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
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float loc = (sin(timeValue) / 3.0f);
    float vertices[] = {
        loc,  loc, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
        loc, -loc, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -loc, -loc, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
        -loc,  loc, 0.0f, 1.0f, 0.0f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
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
    
    _shader->use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}