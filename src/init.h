#ifndef INIT_H
#define INIT_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

GLFWwindow* initializeGlfwWindow();

void onWindowResize(GLFWwindow* window, int width, int height);

void setupImGui(GLFWwindow* window);

void setupInitialViewport(GLFWwindow* window);

#endif