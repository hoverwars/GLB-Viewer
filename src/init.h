#ifndef INIT_H
#define INIT_H

#include <GLFW/glfw3.h>

GLFWwindow* initializeGlfwWindow();

void onWindowResize(GLFWwindow* window, int width, int height);

void setupImGui(GLFWwindow* window);

void setupInitialViewport(GLFWwindow* window);

void setupIconPack();

#endif