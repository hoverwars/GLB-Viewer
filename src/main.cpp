#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "init.h"
#include "views/app.h"
#include "utils/logs.h"


int main()
{
    logInfo("Starting application...");
    GLFWwindow* window = initializeGlfwWindow();
    if (window == NULL)
    {
        logError("Failed to create GLFW window.");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    logInfo("Widow opened succesfully.");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logError("Failed to initialize GLAD.");
        glfwTerminate();
        return -1;
    }

    setupInitialViewport(window);
    glfwSetFramebufferSizeCallback(window, onWindowResize); // handle resize

    setupImGui(window);
    logInfo("UI framework ready.");

    App app = App(window);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering here
        app.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // (Your code calls glfwSwapBuffers() etc.)
        glfwSwapBuffers(window);
    }
    
    logInfo("Closing application...");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
