#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/IconsLucide.h"

#include "init.h"

using namespace std;

GLFWwindow* initializeGlfwWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    return glfwCreateWindow(1080, 720, "GLB Viewer", NULL, NULL);
}

void onWindowResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void setupImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void setupInitialViewport(GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
}

void setupIconPack()
{
    ImGuiIO& io = ImGui::GetIO();
    float baseFontSize = 13.0f; // 13.0f is the size of the default font. Change to the font size you use.
    ImFontConfig base_config;
    base_config.SizePixels = baseFontSize; // Explicit size required to merge another font (ImGui >= 1.92)
    io.Fonts->AddFontDefault(&base_config);
    float iconFontSize = 14.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    // merge in icons from Font Awesome
    static const ImWchar icons_ranges[] = { ICON_MIN_LC, ICON_MAX_LC, 0 };
    ImFontConfig icons_config; 
    icons_config.MergeMode = true; 
    icons_config.PixelSnapH = true; 
    icons_config.GlyphMinAdvanceX = iconFontSize;

    icons_config.GlyphOffset = ImVec2(0.0f, 4.0f);
    io.Fonts->AddFontFromFileTTF("resource/fonts/" FONT_ICON_FILE_NAME_LC, iconFontSize, &icons_config, icons_ranges );
}